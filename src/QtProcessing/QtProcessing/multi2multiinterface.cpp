#include "multi2multiinterface.h"
#include "ui_multi2multiinterface.h"
#include <Plugin/pluginfactory.h>
#include <QtIO/output.h>
#include <QtProcessing/Multi2Multi/maxstacking.h>
#include <QtProcessing/Multi2Multi/medianstacking.h>
#include <QtProcessing/Multi2Multi/reader.h>
#include <QtProcessing/Multi2Multi/robustmeanstacking.h>
#include <QtProcessing/Multi2Multi/starregister.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

namespace
{
std::map<QString, Multi2MultiInterface*> scanPlugins()
{
    std::map<QString, Multi2MultiInterface*> plugins;
    {
        auto* reader = new Reader();
        plugins.emplace(reader->name(), reader);
    }
    {
        auto* stacker = new MaxStacking();
        plugins.emplace(stacker->name(), stacker);
    }
    {
        auto* stacker = new MedianStacking();
        plugins.emplace(stacker->name(), stacker);
    }
    {
        auto* stacker = new RobustMeanStacking();
        plugins.emplace(stacker->name(), stacker);
    }
    {
        auto* starRegister = new StarRegister();
        plugins.emplace(starRegister->name(), starRegister);
    }
    for (auto object : PluginFactory::get().getPluginsFor<Multi2MultiInterface*>())
    {
        auto* plugin = qobject_cast<Multi2MultiInterface*>(object);
        if (plugin != nullptr)
        {
            plugins.emplace(plugin->name(), plugin);
        }
    }
    return plugins;
}
} // namespace

Multi2MultiInterface::Multi2MultiInterface() = default;
Multi2MultiInterface::~Multi2MultiInterface() = default;

const std::map<QString, Multi2MultiInterface*>& Multi2MultiInterface::getPlugins()
{
    static std::map<QString, Multi2MultiInterface*> plugins = scanPlugins();
    return plugins;
}

Multi2MultiInterfaceGUI::Multi2MultiInterfaceGUI(QWidget* parent)
    : QGroupBox(parent)
    , m_multiUi(std::make_unique<Ui::Multi2MultiInterface>())
{
}

Multi2MultiInterfaceGUI::~Multi2MultiInterfaceGUI() = default;

void Multi2MultiInterfaceGUI::setup(QJsonObject data)
{
    m_name = data["Name"].toString();
}

bool Multi2MultiInterfaceGUI::isActive() const
{
    return isChecked();
}

QString Multi2MultiInterfaceGUI::getName() const
{
    return m_name;
}

size_t Multi2MultiInterfaceGUI::subTasks() const
{
    return 1;
}

} // namespace astro
