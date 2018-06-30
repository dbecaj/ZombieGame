#include "scene.h"

Scene::Scene(SceneType type, QObject *parent) : QGraphicsScene(parent)
{
    this->type = type;
}
