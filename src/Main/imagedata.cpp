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

ImageData::ImageData(QWidget* parent)
    : QFrame(parent), m_ui(std::make_unique<Ui::ImageData>())
{
    m_ui->setupUi(this);
    m_scene = new QGraphicsScene;
    m_ui->graphicsView->setScene(m_scene);

    m_histograms = new QChartView();
    m_ui->verticalLayout->addWidget(m_histograms);

    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("ImageData");
    m_ui->splitter->restoreState(settings.value("splitter").toByteArray());
    m_ui->verticalLayout->restoreState(
        settings.value("verticalLayout").toByteArray());
}

ImageData::~ImageData()
{
    QSettings settings("AstroStack", "AstroStack");
    settings.beginGroup("ImageData");
    settings.setValue("splitter", m_ui->splitter->saveState());
    settings.setValue("verticalLayout", m_ui->verticalLayout->saveState());
}

void ImageData::doubleClicked(const QModelIndex& index)
{
    m_scene->clear();
    QModelIndex current = index;
    QString path(current.data().toString());
    while (current.parent() != QModelIndex())
    {
        current = current.parent();
        path = current.data().toString() + "/" + path;
    }
    QImage image(path);
    m_item = new QGraphicsPixmapItem(QPixmap::fromImage(image));

    m_scene->addItem(m_item);
    handleItem(m_item->pixmap());
}

void ImageData::handleItem(const QPixmap& item)
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

    QChart* chart = new QChart();
    chart->setBackgroundBrush(QColor(0, 0, 0));
    chart->legend()->hide();
    auto serie = new QAreaSeries(seriesRed);
    serie->setColor(QColor(255, 0, 0));
    serie->setBrush(QColor(255, 0, 0, 64));
    chart->addSeries(serie);
    serie = new QAreaSeries(seriesGreen);
    serie->setColor(QColor(0, 255, 0));
    serie->setBrush(QColor(0, 255, 0, 64));
    chart->addSeries(serie);
    serie = new QAreaSeries(seriesBlue);
    serie->setColor(QColor(0, 0, 255));
    serie->setBrush(QColor(0, 0, 255, 64));
    chart->addSeries(serie);
    m_histograms->setChart(chart);
}
