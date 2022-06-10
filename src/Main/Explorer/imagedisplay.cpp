#include "imagedisplay.h"
#include "ui_imagedisplay.h"

#include <IO/input.h>

#include <QtCharts/QAreaSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtConcurrent/QtConcurrentRun>
#include <QtCore/QModelIndex>
#include <QtCore/QSettings>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

namespace astro
{
ImageDisplay::ImageDisplay(QWidget* parent)
    : QFrame(parent)
    , m_ui(std::make_unique<Ui::ImageDisplay>())
{
    m_ui->setupUi(this);
    
    connect(this, &ImageDisplay::signalBadFile, this, &ImageDisplay::msgBadFile);

    m_ui->data->restore("ImageDisplay");
}

ImageDisplay::~ImageDisplay() = default;

void ImageDisplay::display(QString file)
{
    m_progressDialog = new QProgressDialog(tr("Loading in progress."), tr("Cancel"), 0, 2, this);
    m_progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_progressDialog, &QProgressDialog::canceled, &m_watcher, &QFutureWatcher<void>::cancel);
    connect(&m_watcher, &QFutureWatcher<void>::progressValueChanged, m_progressDialog, &QProgressDialog::setValue);

    m_watcher.setFuture(QtConcurrent::run([=](QPromise<void>& promise) { process(file, promise); }));
    m_progressDialog->show();
}

void ImageDisplay::process(QString file, QPromise<void>& promise)
{
    m_img = InputInterface::loadImg(file, this);
    emit promise.setProgressValue(1);
    if (promise.isCanceled())
    {
        return;
    }
    if (m_img)
    {
        m_ui->data->handleItem(m_img);
        emit promise.setProgressValue(2);
    }
    else
    {
        emit promise.setProgressValue(2);
        emit signalBadFile(file);
    }
    emit m_progressDialog->finished(2);
}

void ImageDisplay::msgBadFile(QString file)
{
    QMessageBox msgBox;
    msgBox.setText(tr("Could not load image ") + file + ".");
    msgBox.exec();
    m_progressDialog->deleteLater();
}

} // namespace astro
