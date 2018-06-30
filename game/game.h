#ifndef MAINGAME_H
#define MAINGAME_H

#include <QList>
#include <QWidget>
#include <QGraphicsScene>

#include "game/misc/level.h"
#include "game/ui/score.h"
#include "core/scene.h"

#define FPS 60.0f

enum class GameState
{
    RUNNING,
    STOPPED,
    PAUSED,
    WIN,
    LOSE,
    OUT_OF_TIME
};

enum ObjectType
{
    ENTITY = 0,
    TILE = 1
};

class QCloseEvent;
class Player;
class QKeyEvent;
class Tile;
class Scientist;
class Zombie;
class QMouseEvent;
class QTimer;
class InputManager;
class PauseMenu;
class Mob;

class Game : public Scene
{
    Q_OBJECT

    float fpsTime;

    void updateEntities();
    void endGame(); // Stops the gameLoop and writes out end message

protected:
    QTimer* gameLoopTimer;
    bool reset; // Used to know if we already ran this game

    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent* event);
    void handleInput(); // Handles player input
    void handleCollision();
    void sortForDrawing();
public slots:
    bool gameLoop(); // This is main game loop and its run at 60FPS
    void zombieMobHit(Mob* mob);
    void playerMobHit(Mob *mob);
public:
    explicit Game(QGraphicsView *parent = 0, SceneType type = SceneType::GAME);
    ~Game();

    virtual void init();
    void init(QString levelPath);
    virtual void cleanUp(); // Stops the gameLoop and deletes all QGraphicsItems in the scene and does general clean up
    void stopGameLoop();
    virtual void quitGame(); // This calles cleanUp() and changes the scene to mainMenu

    virtual QList<Scientist*> getScientists() { return scientists; }

    virtual QList<Mob*> getMobs();

    // PS: Maybe set getters and setters?
    PauseMenu* pauseMenu;
    InputManager* inputManager;
    Player* player;
    QList<Tile*> tiles;
    Level level;
    QList<Scientist*> scientists;
    QList<Zombie*> zombies;
    long long frameCounter;
    Score* score;
    UIManager* ui;
    GameState gameState;
};

#endif // MAINGAME_H
