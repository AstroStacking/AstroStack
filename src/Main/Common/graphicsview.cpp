#include "graphicsview.h"

#include <QtGui/QWheelEvent>

namespace astro
{
GraphicsView::GraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
}

void GraphicsView::wheelEvent(QWheelEvent* wheelEvent)
{
    if (wheelEvent->modifiers() & Qt::ControlModifier)
    {
        // zoom
        const ViewportAnchor anchor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = wheelEvent->pixelDelta().y();
        qreal factor;
        if (angle > 0)
        {
            factor = 1.1;
        }
        else
        {
            factor = 0.9;
        }
        scale(factor, factor);
        setTransformationAnchor(anchor);
    }
    else
    {
        QGraphicsView::wheelEvent(wheelEvent);
    }
}
} // namespace astro
