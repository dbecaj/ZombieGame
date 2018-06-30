#ifndef LEVELCREATOR_H
#define LEVELCREATOR_H

#include <QGraphicsScene>
#include <QTimer>

#include "core/scene.h"
#include "misc/mousecontroller.h"

#define FPS 60.0f

class PauseMenu;
class UIManager;
class BottomBar;
class Grid;
class TileMover;

class LevelCreator : public Scene
{
    Q_OBJECT

    QString levelName;
    int levelWidth;
    int levelHeight;
    PauseMenu* pauseMenu;
    bool paused;
    QTimer* gameTimer;
    MouseController controller;
    float fpsTime;
    UIManager* uiManager;
    BottomBar* bottomBar;
    Grid* grid;
    TileMover* mover;
    bool removeMode;

    void cleanUp();

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
public slots:
    void gameLoop();
    void saveButtonClick();
public:
    explicit LevelCreator(QGraphicsView *parent = nullptr);

    void init();
    void init(QString levelName, int levelWidth, int levelHeight);
    void checkForPlayerTile();

    void setRemoveMode(bool removeMode) { this->removeMode = removeMode; }

    int getLevelWidth() { return levelWidth; }
    int getLevelHeight() { return levelHeight; }
    Grid* getGrid() { return grid; }
    TileMover* getMover() { return mover; }
    BottomBar* getBottomBar() { return bottomBar; }
    bool isInRemoveMode() { return removeMode; }
};

#endif // LEVELCREATOR_H
