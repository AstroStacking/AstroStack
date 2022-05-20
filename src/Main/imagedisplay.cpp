#include "imagedisplay.h"
#include "ui_imagedisplay.h"
#include <pluginfactory.h>

#include <itkInput.h>

#include <QtCharts/QAreaSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QFileInfo>
#include <QtCore/QModelIndex>
#include <QtCore/QSettings>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsView>
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

ImageTypePtr ImageDisplay::loadImg(QString path)
{
    QFileInfo info(path);
    QString extension = info.completeSuffix();

    const auto& plugins = PluginFactory::get().getPluginsFor<InputInterface*>();
    for (auto object : plugins)
    {
        auto* plugin = qobject_cast<InputInterface*>(object);
        if (plugin->filters().count(extension))
        {
            ImageTypePtr img = plugin->open(path, this);
            if (img)
            {
                return img;
            }
        }
    }
    return {};
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
    m_img = loadImg(path);
    if (m_img)
    {
        m_ui->data->handleItem(m_img);
    }
}
} // namespace astro
