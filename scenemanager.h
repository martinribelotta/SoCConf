#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <QGraphicsScene>

class SceneManager : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SceneManager(QObject *parent = 0l);
    virtual ~SceneManager();

private:
    struct priv_t;
    priv_t *priv;
};

#endif // SCENEMANAGER_H
