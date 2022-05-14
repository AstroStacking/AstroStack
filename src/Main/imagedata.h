#pragma once
#include <QtWidgets/QFrame>

namespace Ui
{
class ImageData;
}

class QChartView;
class QTableWidget;
class QGraphicsScene;

class ImageData : public QFrame
{
    Q_OBJECT

  public:
    explicit ImageData(QWidget* parent = nullptr);
    ~ImageData();

  public slots:
    void doubleClicked(const QModelIndex& index);

  private:
    std::unique_ptr<Ui::ImageData> m_ui;

    QChartView* m_histograms{};
    QTableWidget* m_stats{};
    QGraphicsScene* m_scene{};
};
