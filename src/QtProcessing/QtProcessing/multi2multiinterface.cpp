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

void Multi2MultiInterfaceGUI::setupSlots()
{
    connect(m_multiUi->saveOutput, &QCheckBox::stateChanged, this, &Multi2MultiInterfaceGUI::outputStateChanged);
    connect(m_multiUi->filenameOpen, &QPushButton::clicked, this, &Multi2MultiInterfaceGUI::outputFileBoxOpen);
    connect(this, &Multi2MultiInterfaceGUI::save, this, &Multi2MultiInterfaceGUI::saveImg);
}

bool Multi2MultiInterfaceGUI::isActive()
{
    return isChecked();
}
void Multi2MultiInterfaceGUI::outputStateChanged(int state)
{
    m_multiUi->filename->setEnabled(state != Qt::Unchecked);
    m_multiUi->filenameOpen->setEnabled(state != Qt::Unchecked);
}

void Multi2MultiInterfaceGUI::outputFileBoxOpen()
{
    m_multiUi->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_multiUi->filename->text()));
}

bool Multi2MultiInterfaceGUI::check()
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

void Multi2MultiInterfaceGUI::setNextFilename(QFileInfo info, QString basename, long inc)
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

void Multi2MultiInterfaceGUI::saveImg(const AstroImage& img)
{
    QFileInfo info(m_multiUi->filename->text());
    if (m_multiUi->saveOutput->checkState() == Qt::Checked && (!info.exists() || m_overwriteIfExists))
    {
        OutputInterface::saveImg(img, m_multiUi->filename->text(), this);
    }
}

} // namespace astro
