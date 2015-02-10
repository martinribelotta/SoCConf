#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QGraphicsScene>

class SceneManager : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SceneManager(QObject *parent = 0l);
    virtual ~SceneManager();
};

#endif // SCENEMANAGER_H
