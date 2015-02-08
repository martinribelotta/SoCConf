#include "scenemanager.h"

#include "diagramelements.h"

#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>

#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QtScriptTools>
#include <QFile>

#include <QtDebug>

struct SceneManager::priv_t {

};

SceneManager::SceneManager(QObject *parent) :
    QGraphicsScene(parent),
    priv(new priv_t)
{
}

SceneManager::~SceneManager()
{
    delete priv;
}
