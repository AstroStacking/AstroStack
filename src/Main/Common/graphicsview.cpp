#include "graphicsview.h"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QGraphicsPixmapItem>

namespace astro
{
GraphicsView::GraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    QSize oldSize = size();
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
        m_factor *= factor;
        setTransformationAnchor(anchor);
    }
    else
    {
        QGraphicsView::wheelEvent(wheelEvent);
    }
}

void GraphicsView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    if (items().empty())
    {
        return;
    }
    QSize pixmapSize = static_cast<QGraphicsPixmapItem*>(items().front())->pixmap().size();
    QSize newSize = event->size();
    resetTransform();
    double factor = std::min(static_cast<double>(newSize.width()) / pixmapSize.width(),
                             static_cast<double>(newSize.height()) / pixmapSize.height());
    scale(factor * m_factor, factor * m_factor);
}

void GraphicsView::reset()
{
    m_factor = 1;
}

} // namespace astro
