#pragma once
#include <io.h>

#include <QtWidgets/QFrame>

namespace Ui
{
class ImageDisplay;
}

class QChartView;
class QTableWidget;
class QGraphicsScene;
class QGraphicsPixmapItem;

namespace astro
{
class ImageDisplay : public QFrame
{
    Q_OBJECT

    ImageTypePtr loadImg(QString path);

public:
    explicit ImageDisplay(QWidget* parent = nullptr);
    ~ImageDisplay();

    void display(QString file);

private:
    std::unique_ptr<Ui::ImageDisplay> m_ui;

    ImageTypePtr m_img;
};
} // namespace astro
