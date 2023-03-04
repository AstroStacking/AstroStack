#include "multi2monointerface.h"
#include "ui_multi2monointerface.h"
#include <Plugin/pluginfactory.h>
#include <QtIO/output.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

namespace astro
{

namespace
{
std::map<QString, Multi2MonoInterface*> scanPlugins()
{
    std::map<QString, Multi2MonoInterface*> plugins;
    return plugins;
}
} // namespace

Multi2MonoInterface::Multi2MonoInterface() = default;
Multi2MonoInterface::~Multi2MonoInterface() = default;

const std::map<QString, Multi2MonoInterface*>& Multi2MonoInterface::getPlugins()
{
    static std::map<QString, Multi2MonoInterface*> plugins = scanPlugins();
    return plugins;
}

Multi2MonoInterfaceGUI::Multi2MonoInterfaceGUI(QWidget* parent)
    : QGroupBox(parent)
    , m_multiUi(std::make_unique<Ui::Multi2MonoInterface>())
{
}

Multi2MonoInterfaceGUI::~Multi2MonoInterfaceGUI() = default;

void Multi2MonoInterfaceGUI::setup(QJsonObject data)
{
    m_name = data["Name"].toString();
}

void Multi2MonoInterfaceGUI::setupSlots()
{
    connect(m_multiUi->saveOutput, &QCheckBox::stateChanged, this, &Multi2MonoInterfaceGUI::outputStateChanged);
    connect(m_multiUi->filenameOpen, &QPushButton::clicked, this, &Multi2MonoInterfaceGUI::outputFileBoxOpen);
    connect(this, &Multi2MonoInterfaceGUI::save, this, &Multi2MonoInterfaceGUI::saveImg);
}

bool Multi2MonoInterfaceGUI::isActive()
{
    return isChecked();
}
void Multi2MonoInterfaceGUI::outputStateChanged(int state)
{
    m_multiUi->filename->setEnabled(state != Qt::Unchecked);
    m_multiUi->filenameOpen->setEnabled(state != Qt::Unchecked);
}

void Multi2MonoInterfaceGUI::outputFileBoxOpen()
{
    m_multiUi->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_multiUi->filename->text()));
}

bool Multi2MonoInterfaceGUI::check()
{
    QFileInfo info(m_multiUi->filename->text());
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
        QFileInfo info(m_multiUi->filename->text());
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

void Multi2MonoInterfaceGUI::setNextFilename(QFileInfo info, QString basename, long inc)
{
    while (true)
    {
        QString potentialFilename =
                info.dir().filePath(basename + "_" + QString::number(inc) + "." + info.completeSuffix());
        if (!QFileInfo(potentialFilename).exists())
        {
            m_multiUi->filename->setText(potentialFilename);
            break;
        }
        ++inc;
    }
}

void Multi2MonoInterfaceGUI::saveImg(const AstroImage& img)
{
    QFileInfo info(m_multiUi->filename->text());
    if (m_multiUi->saveOutput->checkState() == Qt::Checked && (!info.exists() || m_overwriteIfExists))
    {
        OutputInterface::saveImg(img, m_multiUi->filename->text(), this);
    }
}

} // namespace astro
