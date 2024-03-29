#pragma once
#include <IO/io.h>

#include <QtCharts/QChart>
#include <QtWidgets/QFrame>

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

    QGraphicsPixmapItem* processImg(const ImageTypePtr& img);
    void processItem(const QPixmap& item);

public:
    explicit ImageData(QWidget* parent = nullptr);
    ~ImageData();
    void restore(QString entry);

    void handleItem(const AstroImage& img);

signals:
    void triggerSwapItem();

private slots:
    void swapItem();

private:
    void save();

    std::unique_ptr<Ui::ImageData> m_ui;

    QChartView* m_histograms{};
    QGraphicsScene* m_scene{};
    QGraphicsPixmapItem* m_item{};
    std::unique_ptr<QChart> m_chart;
    QString m_entry;
};
} // namespace astro
