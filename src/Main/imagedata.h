#pragma once
#include <io.h>

#include <QtWidgets/QFrame>
#include <QtCharts/QChart>

namespace Ui
{
class ImageData;
}

class QChartView;
class QTableWidget;
class QGraphicsScene;
class QGraphicsPixmapItem;

namespace astro
{
class ImageData : public QFrame
{
    Q_OBJECT

    template<int PIXEL_SIZE>
    QGraphicsPixmapItem* processImg(const ImageTypePtr& img);
    void processItem(const QPixmap& item);
public:
    explicit ImageData(QWidget* parent = nullptr);
    ~ImageData();

    void handleItem(ImageTypePtr img);

private:
    std::unique_ptr<Ui::ImageData> m_ui;

    QChartView* m_histograms{};
    QGraphicsScene* m_scene{};
    QGraphicsPixmapItem* m_item{};
    std::unique_ptr<QChart> m_chart;
};
} // namespace astro
