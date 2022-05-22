#pragma once
#include <IO/io.h>

#include <QtCore/QFutureWatcher>
#include <QtWidgets/QFrame>

namespace Ui
{
class ImageDisplay;
}

class QChartView;
class QProgressDialog;
class QGraphicsScene;
class QGraphicsPixmapItem;
class QTableWidget;

namespace astro
{
class ImageDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit ImageDisplay(QWidget* parent = nullptr);
    ~ImageDisplay();

    void display(QString file);

private:
    void process(QString file, QPromise<void>& promise);

    std::unique_ptr<Ui::ImageDisplay> m_ui;
    QProgressDialog* m_progressDialog{};
    QFutureWatcher<void> m_watcher;

    ImageTypePtr m_img;
};
} // namespace astro
