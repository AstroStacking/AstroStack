#include "imagedisplay.h"
#include "ui_imagedisplay.h"

#include <itkInput.h>

#include <QtCharts/QAreaSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QModelIndex>
#include <QtCore/QSettings>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

namespace astro
{
ImageDisplay::ImageDisplay(QWidget* parent)
    : QFrame(parent), m_ui(std::make_unique<Ui::ImageDisplay>())
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

void ImageDisplay::doubleClicked(const QModelIndex& index)
{
    QModelIndex current = index;
    QString path(current.data().toString());
    while (current.parent() != QModelIndex())
    {
        current = current.parent();
        path = current.data().toString() + "/" + path;
    }
    
    ITKInputPlugin plugin;
    m_img = plugin.open(path, this);
}
}
