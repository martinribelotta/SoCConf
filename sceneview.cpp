#include "sceneview.h"

#include <QWheelEvent>
#include <QScrollBar>

#include <QtDebug>

#include <cmath>

SceneView::SceneView(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);
}

SceneView::~SceneView()
{
}

void SceneView::wheelEvent(QWheelEvent *event)
{
    int d = event->delta();
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        const QPointF p0scene = mapToScene(event->pos());

        qreal factor = std::pow(1.001, event->delta());
        scale(factor, factor);

        const QPointF p1mouse = mapFromScene(p0scene);
        const QPointF move = p1mouse - event->pos(); // The move
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
    } else if (event->modifiers().testFlag(Qt::ShiftModifier)) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - d);
    } else {
        verticalScrollBar()->setValue(verticalScrollBar()->value() - d);
    }
}

