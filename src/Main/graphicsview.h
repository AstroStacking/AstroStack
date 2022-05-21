#include <QtWidgets/QGraphicsView>

namespace astro
{
class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget* parent = nullptr);
    void wheelEvent(QWheelEvent* wheelEvent) override;
};
} // namespace astro
