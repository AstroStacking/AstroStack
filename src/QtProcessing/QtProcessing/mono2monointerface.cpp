#include "mono2monointerface.h"
#include "ui_mono2monointerface.h"
#include <Plugin/pluginfactory.h>
#include <QtIO/output.h>
#include <QtProcessing/Mono2Mono/chromasmoothing.h>
#include <QtProcessing/Mono2Mono/exponential.h>
#include <QtProcessing/Mono2Mono/histostretch.h>
#include <QtProcessing/Mono2Mono/lightpollution.h>
#include <QtProcessing/Mono2Mono/rldeconvolution.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

namespace
{
std::map<QString, Mono2MonoInterface*> scanPlugins()
{
    std::map<QString, Mono2MonoInterface*> plugins;
    {
        auto* chromasmoothing = new ChromaSmoothing();
        plugins.emplace(chromasmoothing->name(), chromasmoothing);
    }
    {
        auto* exponential = new Exponential();
        plugins.emplace(exponential->name(), exponential);
    }
    {
        auto* histostretch = new HistoStretch();
        plugins.emplace(histostretch->name(), histostretch);
    }
    {
        auto* lightpollution = new LightPollution();
        plugins.emplace(lightpollution->name(), lightpollution);
    }
    {
        auto* rldeconvolution = new RLDeconvolution();
        plugins.emplace(rldeconvolution->name(), rldeconvolution);
    }
    for (auto object : PluginFactory::get().getPluginsFor<Mono2MonoInterface*>())
    {
        auto* plugin = qobject_cast<Mono2MonoInterface*>(object);
        plugins.emplace(plugin->name(), plugin);
    }
    return plugins;
}
} // namespace

Mono2MonoInterface::Mono2MonoInterface() = default;
Mono2MonoInterface::~Mono2MonoInterface() = default;

const std::map<QString, Mono2MonoInterface*>& Mono2MonoInterface::getPlugins()
{
    static std::map<QString, Mono2MonoInterface*> plugins = scanPlugins();
    return plugins;
}

Mono2MonoInterfaceGUI::Mono2MonoInterfaceGUI(QWidget* parent)
    : QGroupBox(parent)
    , m_monoUi(std::make_unique<Ui::Mono2MonoInterface>())
{
}

Mono2MonoInterfaceGUI::~Mono2MonoInterfaceGUI() = default;

void Mono2MonoInterfaceGUI::setup(QJsonObject data)
{
    m_name = data["Name"].toString();
}

void Mono2MonoInterfaceGUI::setupSlots()
{
    connect(m_monoUi->saveOutput, &QCheckBox::stateChanged, this, &Mono2MonoInterfaceGUI::outputStateChanged);
    connect(m_monoUi->filenameOpen, &QPushButton::clicked, this, &Mono2MonoInterfaceGUI::outputFileBoxOpen);
    connect(this, &Mono2MonoInterfaceGUI::save, this, &Mono2MonoInterfaceGUI::saveImg);
}

bool Mono2MonoInterfaceGUI::isActive()
{
    return isChecked();
}
void Mono2MonoInterfaceGUI::outputStateChanged(int state)
{
    m_monoUi->filename->setEnabled(state != Qt::Unchecked);
    m_monoUi->filenameOpen->setEnabled(state != Qt::Unchecked);
}

void Mono2MonoInterfaceGUI::outputFileBoxOpen()
{
    m_monoUi->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_monoUi->filename->text()));
}

bool Mono2MonoInterfaceGUI::check()
{
    QFileInfo info(m_monoUi->filename->text());
    if (!info.exists())
    {
        return true;
    }
    int result = QMessageBox::question(this, m_name, tr("Overwrite existing file?"),
                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
    if (result == QMessageBox::Abort)
    {
        return false;
    }
    m_overwriteIfExists = result == QMessageBox::Yes;
    if (!m_overwriteIfExists)
    {
        QFileInfo info(m_monoUi->filename->text());
        QString basename = info.completeBaseName();
        QRegularExpression regex("(.*)_(\\d+)$");
        auto match = regex.match(basename);
        if (match.hasMatch())
        {
            setNextFilename(info, match.captured(1), match.captured(2).toLong());
        }
        else
        {
            setNextFilename(info, basename, 1);
        }
    }

    return true;
}

void Mono2MonoInterfaceGUI::setNextFilename(QFileInfo info, QString basename, long inc)
{
    while (true)
    {
        QString potentialFilename =
                info.dir().filePath(basename + "_" + QString::number(inc) + "." + info.completeSuffix());
        if (!QFileInfo(potentialFilename).exists())
        {
            m_monoUi->filename->setText(potentialFilename);
            break;
        }
        ++inc;
    }
}

void Mono2MonoInterfaceGUI::saveImg(const AstroImage& img)
{
    QFileInfo info(m_monoUi->filename->text());
    if (m_monoUi->saveOutput->checkState() == Qt::Checked && (!info.exists() || m_overwriteIfExists))
    {
        OutputInterface::saveImg(img, m_monoUi->filename->text(), this);
    }
}

} // namespace astro
