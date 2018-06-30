#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

enum class SceneType
{
    INPUT_PAGE,
    MAIN_MENU,
    LEVEL_SELECTOR,
    GAME,
    MULTIPLAYER_PAGE,
    MULTIPLAYER_GAME,
    LEVEL_SIZE_PAGE,
    LEVEL_CREATOR
};

class Scene : public QGraphicsScene
{
    SceneType type;
public:
    Scene(SceneType type, QObject* parent = nullptr);

    virtual void init() = 0;
    virtual void cleanUp() = 0;

    SceneType getSceneType() { return type; }
};

#endif // SCENE_H
