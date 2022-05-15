#pragma once
#include <QtWidgets/QFrame>

namespace Ui
{
class ImageData;
}

class QChartView;
class QTableWidget;
class QGraphicsScene;
class QGraphicsPixmapItem;

class ImageData : public QFrame
{
    Q_OBJECT

  public:
    explicit ImageData(QWidget* parent = nullptr);
    ~ImageData();

  public slots:
    void doubleClicked(const QModelIndex& index);

  private:
    void handleItem(const QPixmap& item);

    std::unique_ptr<Ui::ImageData> m_ui;

    QChartView* m_histograms{};
    QGraphicsScene* m_scene{};
    QGraphicsPixmapItem* m_item{};
};
