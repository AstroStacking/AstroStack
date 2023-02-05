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
std::map<QString, MultiInterface*> scanPlugins()
{
    std::map<QString, MultiInterface*> plugins;
    return plugins;
}
} // namespace

MultiInterface::MultiInterface() = default;
MultiInterface::~MultiInterface() = default;

const std::map<QString, MultiInterface*>& MultiInterface::getPlugins()
{
    static std::map<QString, MultiInterface*> plugins = scanPlugins();
    return plugins;
}

MultiInterfaceGUI::MultiInterfaceGUI(QWidget* parent)
    : QGroupBox(parent)
    , m_multiUi(std::make_unique<Ui::MultiInterface>())
{
}

MultiInterfaceGUI::~MultiInterfaceGUI() = default;

void MultiInterfaceGUI::setup(QJsonObject data)
{
    m_name = data["Name"].toString();
}

void MultiInterfaceGUI::setupSlots()
{
    connect(m_multiUi->saveOutput, &QCheckBox::stateChanged, this, &MultiInterfaceGUI::outputStateChanged);
    connect(m_multiUi->filenameOpen, &QPushButton::clicked, this, &MultiInterfaceGUI::outputFileBoxOpen);
    connect(this, &MultiInterfaceGUI::save, this, &MultiInterfaceGUI::saveImg);
}

bool MultiInterfaceGUI::isActive()
{
    return isChecked();
}
void MultiInterfaceGUI::outputStateChanged(int state)
{
    m_multiUi->filename->setEnabled(state != Qt::Unchecked);
    m_multiUi->filenameOpen->setEnabled(state != Qt::Unchecked);
}

void MultiInterfaceGUI::outputFileBoxOpen()
{
    m_multiUi->filename->setText(QFileDialog::getSaveFileName(this, tr("Save output"), m_multiUi->filename->text()));
}

bool MultiInterfaceGUI::check()
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

void MultiInterfaceGUI::setNextFilename(QFileInfo info, QString basename, long inc)
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

void MultiInterfaceGUI::saveImg(const AstroImage& img)
{
    QFileInfo info(m_multiUi->filename->text());
    if (m_multiUi->saveOutput->checkState() == Qt::Checked && (!info.exists() || m_overwriteIfExists))
    {
        OutputInterface::saveImg(img, m_multiUi->filename->text(), this);
    }
}

} // namespace astro
