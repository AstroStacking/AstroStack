#pragma once
#include <QFrame>

namespace Ui
{
class ImageData;
}

class QChartView;
class QTableWidget;

class ImageData : public QFrame
{
    Q_OBJECT

  public:
    explicit ImageData(QWidget* parent = nullptr);
    ~ImageData();

  private:
    std::unique_ptr<Ui::ImageData> m_ui;

    QChartView* m_histograms{};
    QTableWidget* m_stats{};
};
