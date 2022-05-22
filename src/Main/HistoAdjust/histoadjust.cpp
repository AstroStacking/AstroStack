#include "histoadjust.h"
#include "ui_histoadjust.h"

#include <Common/imagedata.h>
#include <IO/input.h>

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QFileSystemModel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>

namespace astro
{
HistoAdjust::HistoAdjust(QString filename, QWidget* parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::HistoAdjust>())
{
    m_ui->setupUi(this);
    setWindowTitle(filename);

    restore();

    m_img = InputInterface::loadImg(filename, this);
    if (m_img)
    {
        m_ui->input->handleItem(m_img);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Could not load image " + filename + ".");
        msgBox.exec();
    }
}

HistoAdjust::~HistoAdjust()
{
    save();
}

void HistoAdjust::restore()
{
    m_ui->input->restore("ImageDisplayInput");
    m_ui->output->restore("ImageDisplayOutput");

    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
    if (!settings.contains("geometry"))
    {
        return;
    }
    restoreGeometry(settings.value("geometry").toByteArray());
    m_ui->splitter->restoreState(settings.value("splitter").toByteArray());
    settings.endGroup();
}

void HistoAdjust::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("HistoAdjust");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", m_ui->splitter->saveState());
    settings.endGroup();
}
} // namespace astro
