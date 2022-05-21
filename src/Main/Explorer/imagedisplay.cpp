#include "imagedisplay.h"
#include "ui_imagedisplay.h"

#include <IO/input.h>

#include <QtCharts/QAreaSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QModelIndex>
#include <QtCore/QSettings>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

namespace astro
{
ImageDisplay::ImageDisplay(QWidget* parent)
    : QFrame(parent)
    , m_ui(std::make_unique<Ui::ImageDisplay>())
{
    m_ui->setupUi(this);

    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("ImageDisplay");
}

ImageDisplay::~ImageDisplay()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("ImageDisplay");
}

void ImageDisplay::display(QString file)
{
    m_img = InputInterface::loadImg(file, this);
    if (m_img)
    {
        m_ui->data->handleItem(m_img);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Could not load image " + file + ".");
        msgBox.exec();
    }
}
} // namespace astro
