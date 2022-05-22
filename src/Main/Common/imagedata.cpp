#include "imagedata.h"
#include "ui_imagedata.h"

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
namespace
{
template<int size>
QRgba64 from(ImageType::PixelType pixel);

template<>
QRgba64 from<1>(ImageType::PixelType pixel)
{
    return QRgba64::fromRgba64(static_cast<quint16>(pixel[0] * 65535), static_cast<quint16>(pixel[0] * 65535),
                               static_cast<quint16>(pixel[0] * 65535), 65535);
}

template<>
QRgba64 from<3>(ImageType::PixelType pixel)
{
    return QRgba64::fromRgba64(static_cast<quint16>(pixel[0] * 65535), static_cast<quint16>(pixel[1] * 65535),
                               static_cast<quint16>(pixel[2] * 65535), 65535);
}

template<>
QRgba64 from<4>(ImageType::PixelType pixel)
{
    return QRgba64::fromRgba64(static_cast<quint16>(pixel[0] * 65535), static_cast<quint16>(pixel[1] * 65535),
                               static_cast<quint16>(pixel[2] * 65535), static_cast<quint16>(pixel[3] * 65535));
}

} // namespace

ImageData::ImageData(QWidget* parent)
    : QFrame(parent)
    , m_ui(std::make_unique<Ui::ImageData>())
{
    m_ui->setupUi(this);
    m_scene = new QGraphicsScene;
    m_ui->graphicsView->setScene(m_scene);

    m_histograms = new QChartView();
    m_chart = std::make_unique<QChart>();
    m_histograms->setChart(m_chart.get());
    m_ui->verticalLayout->addWidget(m_histograms);
}

ImageData::~ImageData()
{
    save();
}

void ImageData::restore(QString entry)
{
    m_entry = entry;
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup(m_entry);
    m_ui->splitter->restoreState(settings.value("splitter").toByteArray());
    m_ui->verticalLayout->restoreState(settings.value("verticalLayout").toByteArray());
    QDataStream stream(settings.value("transform").toByteArray());
    if (!stream.atEnd())
    {
        QTransform transform;
        stream >> transform;
        m_ui->graphicsView->setTransform(transform);
    }
}

void ImageData::save()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup(m_entry);
    settings.setValue("splitter", m_ui->splitter->saveState());
    settings.setValue("verticalLayout", m_ui->verticalLayout->saveState());
    QDataStream stream;
    QTransform transform = m_ui->graphicsView->transform();
    stream << transform;
    QByteArray array;
    stream >> array;
    settings.setValue("transform", array);
}

template<int PIXEL_SIZE>
QGraphicsPixmapItem* ImageData::processImg(const ImageTypePtr& img)
{
    ImageType::RegionType region = img->GetLargestPossibleRegion();
    ImageType::SizeType size = region.GetSize();
    QImage image(size[0], size[1], QImage::Format_ARGB32);
    ImageType::IndexType index;
    for (int j = 0; j < size[1]; ++j)
    {
        index.SetElement(1, j);
        for (int i = 0; i < size[0]; ++i)
        {
            index.SetElement(0, i);
            image.setPixelColor(i, j, from<PIXEL_SIZE>(img->GetPixel(index)));
        }
    }

    return new QGraphicsPixmapItem(QPixmap::fromImage(image));
}

void ImageData::processItem(const QPixmap& item)
{
    QImage img = item.toImage();
    std::array<std::array<int, 64>, 3> hist{};
    for (int i = 0; i < img.width(); ++i)
    {
        for (int j = 0; j < img.height(); ++j)
        {
            auto pixel = img.pixel(i, j);
            ++hist[0][qRed(pixel) / 4];
            ++hist[1][qGreen(pixel) / 4];
            ++hist[2][qBlue(pixel) / 4];
        }
    }

    QLineSeries* seriesRed = new QLineSeries();
    QLineSeries* seriesGreen = new QLineSeries();
    QLineSeries* seriesBlue = new QLineSeries();
    for (size_t i = 0; i < hist[0].size(); ++i)
    {
        seriesRed->append(i, hist[0][i]);
        seriesGreen->append(i, hist[1][i]);
        seriesBlue->append(i, hist[2][i]);
    }

    m_chart->setBackgroundBrush(QColor(0, 0, 0));
    m_chart->legend()->hide();
    auto serie = new QAreaSeries(seriesRed);
    serie->setColor(QColor(255, 0, 0));
    serie->setBrush(QColor(255, 0, 0, 64));
    m_chart->addSeries(serie);
    serie = new QAreaSeries(seriesGreen);
    serie->setColor(QColor(0, 255, 0));
    serie->setBrush(QColor(0, 255, 0, 64));
    m_chart->addSeries(serie);
    serie = new QAreaSeries(seriesBlue);
    serie->setColor(QColor(0, 0, 255));
    serie->setBrush(QColor(0, 0, 255, 64));
    m_chart->addSeries(serie);
}

void ImageData::handleItem(ImageTypePtr img)
{
    switch (img->GetNumberOfComponentsPerPixel())
    {
        case 1:
            m_item = processImg<1>(img);
            break;
        case 3:
            m_item = processImg<3>(img);
            break;
        case 4:
            m_item = processImg<4>(img);
            break;
        default:
        {
        }
    }
    m_scene->clear();
    m_chart->removeAllSeries();
    if (m_item != nullptr)
    {
        m_scene->addItem(m_item);
        processItem(m_item->pixmap());

        ImageType::RegionType region = img->GetLargestPossibleRegion();
        ImageType::SizeType size = region.GetSize();

        double factor = std::min(static_cast<double>(m_ui->graphicsView->width()) / size.GetElement(0),
                                 static_cast<double>(m_ui->graphicsView->height()) / size.GetElement(1));
        m_ui->graphicsView->setSceneRect(0, 0, size.GetElement(0), size.GetElement(1));
        m_ui->graphicsView->reset();
        m_ui->graphicsView->resetTransform();
        m_ui->graphicsView->scale(factor, factor);
    }
}
} // namespace astro