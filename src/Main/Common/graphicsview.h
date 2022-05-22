#include <QtWidgets/QGraphicsView>

namespace astro
{
class GraphicsView : public QGraphicsView
{
    double m_factor{1};

public:
    GraphicsView(QWidget* parent = nullptr);
    void wheelEvent(QWheelEvent* wheelEvent) override;
    void resizeEvent(QResizeEvent* event) override;

    void reset();
};
} // namespace astro
