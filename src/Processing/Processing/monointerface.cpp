#include "monointerface.h"
#include "ui_monointerface.h"

#include <Processing/exponential.h>
#include <Processing/histostretch.h>

#include <IO/output.h>
#include <Plugin/pluginfactory.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

namespace
{
std::map<QString, MonoInterface*> scanPlugins()
{
    std::map<QString, MonoInterface*> plugins;
    {
        auto* exponential = new Exponential();
        plugins.emplace(exponential->name(), exponential);
    }
    {
        auto* histostretch = new HistoStretch();
        plugins.emplace(histostretch->name(), histostretch);
    }
    for (auto object : PluginFactory::get().getPluginsFor<MonoInterface*>())
    {
        auto* plugin = qobject_cast<MonoInterface*>(object);
        plugins.emplace(plugin->name(), plugin);
    }
    return plugins;
}
} // namespace

MonoInterface::~MonoInterface() = default;


const std::map<QString, MonoInterface*>& MonoInterface::getPlugins()
{
    static std::map<QString, MonoInterface*> plugins = scanPlugins();
    return plugins;
}

MonoInterfaceGUI::MonoInterfaceGUI(QWidget* parent)
    : QGroupBox(parent)
    , m_monoUi(std::make_unique<Ui::MonoInterface>())
{
}

MonoInterfaceGUI::~MonoInterfaceGUI() = default;

void MonoInterfaceGUI::setup(QJsonObject data) {}

void MonoInterfaceGUI::setupSlots()
{
    connect(m_monoUi->saveOutput, &QCheckBox::stateChanged, this, &MonoInterfaceGUI::outputStateChanged);
    connect(m_monoUi->filenameOpen, &QPushButton::clicked, this, &MonoInterfaceGUI::outputFileBoxOpen);
    connect(this, &MonoInterfaceGUI::save, this, &MonoInterfaceGUI::saveImg);
}

bool MonoInterfaceGUI::isActive()
{
    return isChecked();
}
void MonoInterfaceGUI::outputStateChanged(int state)
{
    m_monoUi->filename->setEnabled(state != Qt::Unchecked);
    m_monoUi->filenameOpen->setEnabled(state != Qt::Unchecked);
}

void MonoInterfaceGUI::outputFileBoxOpen()
{
    m_monoUi->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_monoUi->filename->text()));
}

bool MonoInterfaceGUI::check()
{
    QFileInfo info(m_monoUi->filename->text());
    if (!info.exists())
    {
        return true;
    }
    int result = QMessageBox::question(this, tr("Exponential"), tr("Overwrite existing file?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
    if (result == QMessageBox::Abort)
    {
        return false;
    }
    m_overwriteIfExists = result == QMessageBox::Yes;

    return true;
}

void MonoInterfaceGUI::saveImg(ImageTypePtr img)
{
    QFileInfo info(m_monoUi->filename->text());
    if (m_monoUi->saveOutput->checkState() == Qt::Checked && (!info.exists() || m_overwriteIfExists))
    {
        OutputInterface::saveImg(img, m_monoUi->filename->text(), this);
    }
}

} // namespace astro