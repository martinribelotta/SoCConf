#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QWidget>
#include <QGraphicsView>

class SceneView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = 0l);
    virtual ~SceneView();

protected:
    virtual void wheelEvent(QWheelEvent *event);
};

#endif // SCENEVIEW_H
