#include "multi2multiinterface.h"
#include "ui_multi2multiinterface.h"
#include <Plugin/pluginfactory.h>
#include <QtIO/output.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

namespace
{
std::map<QString, Multi2MultiInterface*> scanPlugins()
{
    std::map<QString, Multi2MultiInterface*> plugins;
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

bool Multi2MultiInterfaceGUI::check()
{
    // TODO
    return true;
}

bool Multi2MultiInterfaceGUI::isActive()
{
    return isChecked();
}

} // namespace astro
