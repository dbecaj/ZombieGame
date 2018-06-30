#include "levelcreator.h"

#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsScene>

#include "core/window.h"
#include "main_menu/mainmenu.h"
#include "main_menu/button.h"
#include "game/ui/pausemenu.h"
#include "misc/mousecontroller.h"
#include "game/ui/uimanager.h"
#include "ui/bottombar.h"
#include "grid/grid.h"
#include "grid/gridtile.h"
#include "ui/tilemover.h"
#include "ui/tileitem.h"
#include "grid/leveltile.h"
#include "game/misc/tile.h"
#include "game/misc/level.h"
#include "misc/language.h"
#include "misc/notification.h"

extern Window* w;
LevelCreator::LevelCreator(QGraphicsView *parent) : Scene(SceneType::LEVEL_CREATOR, parent)
{
    this->levelWidth = 0;
    this->levelHeight = 0;
    pauseMenu = nullptr;
    gameTimer = nullptr;
    uiManager = nullptr;
    bottomBar = nullptr;
    mover = nullptr;
    grid = nullptr;
    fpsTime = (1/FPS)*1000.0f;
    removeMode = false;
}

void LevelCreator::init(QString levelName, int levelWidth, int levelHeight)
{
    this->levelName = levelName;
    this->levelWidth = levelWidth;
    this->levelHeight = levelHeight;
    this->setSceneRect(0.0f, 0.0f, levelWidth * TILE_SIZE,
                                   levelHeight * TILE_SIZE);

    init();
}

void LevelCreator::init()
{
    if(!pauseMenu)
    {
        pauseMenu = new PauseMenu(this);
        Button* saveButton = new Button(0.0f, 0.0f, 400.0f, 100.0f, QPixmap(":/images/textures/pause_button_idle.png"),
                                        QPixmap(":/images/textures/pause_button_active.png"), Text(Language::get("save"), QColor(255, 255, 255), QFont("impact", 42)));
        pauseMenu->addButton(saveButton, this, SLOT(saveButtonClick()));
    }
    if(!gameTimer)
    {
        gameTimer = new QTimer(this);
        this->connect(gameTimer, SIGNAL(timeout()), this, SLOT(gameLoop()));
        gameTimer->setInterval(fpsTime);
    }
    if(!uiManager)
    {
        uiManager = new UIManager(this);
        uiManager->setZValue(0.001f);
        this->addItem(uiManager);
    }
    if(!bottomBar)
    {
        bottomBar = new BottomBar(QRectF(0.0f, w->getWindowHeight() - 200.0f, w->getWindowWidth(), 200.0f), uiManager);
        uiManager->addComponent(bottomBar);
    }
    if(!grid)
    {
        grid = new Grid(levelWidth, levelHeight);
        this->addItem(grid);
    }
    if(!mover)
    {
        mover = new TileMover();
    }
    paused = false;

    w->setBackgroundBrush(QBrush(QImage(":/images/textures/tile.png")));

    for(int x = 0; x < levelWidth; x++)
    {
        if(x == 0 || (x + 1) == levelWidth)
        {
            for(int y = 0; y < levelHeight; y++)
            {
                Tile* tile = new Tile(QPixmap(":/images/textures/wall_tile.png"), QPointF(x * TILE_SIZE, y * TILE_SIZE), TileType::WALL);
                this->addItem(tile);
            }
        }
        else
        {
            Tile* tile = new Tile(QPixmap(":/images/textures/wall_tile.png"), QPointF(x * TILE_SIZE, 0.0f), TileType::WALL);
            this->addItem(tile);
            tile = new Tile(QPixmap(":/images/textures/wall_tile.png"), QPointF(x * TILE_SIZE, (levelHeight - 1) * TILE_SIZE), TileType::WALL);
            this->addItem(tile);
        }
    }

    w->setBackgroundBrush(QBrush(QPixmap(":images/textures/tile.png")));

    gameTimer->start();
}

void LevelCreator::saveButtonClick()
{
    if(!grid->getPlayerTile())
    {
        Notification::show(Language::get("put_down_player_tile") + "!", NotificationType::ERROR);
        return;
    }

    Level level;
    level.init(grid->getLevelData());
    level.save(QCoreApplication::applicationDirPath() + "/levels/" + levelName + ".lvl");
    Notification::show(levelName + " " + Language::get("has_been_saved") + "!");
}

void LevelCreator::gameLoop()
{
    controller.update();
    uiManager->update();
    mover->update();
    this->update();
}

void LevelCreator::cleanUp()
{
    gameTimer->stop();

    for(QGraphicsItem* item : items())
        delete item;

    delete mover;

    pauseMenu = nullptr;
    uiManager = nullptr;
    bottomBar = nullptr;
    grid = nullptr;
    mover = nullptr;
}

void LevelCreator::checkForPlayerTile()
{
    GridTile* tile = grid->getPlayerTile();
    if(!tile) return;
    delete tile->getTile();
    tile->unoccupy();
}

void LevelCreator::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        if(!paused)
        {
            QSize windowSize(500, 600);
            QPointF scenePos = w->mapToScene(w->getWindowWidth()/2 - windowSize.width()/2, 50);
            pauseMenu->setPos(scenePos);
            this->addItem(pauseMenu);
            paused = true;
            gameTimer->stop();
        }
        else
        {
            this->removeItem(pauseMenu);
            paused = false;
            gameTimer->start();
        }
    }
}

void LevelCreator::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->accept();
}

void LevelCreator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    w->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    removeMode = false;
    QGraphicsScene::mousePressEvent(event);
}

void LevelCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(mover->hasTile())
    {
        if(mover->getGridTile() && mover->getTile()->getState() == LevelTile::TileState::GREEN)
        {
            if(mover->getTile()->getType() == LevelTile::TileType::PLAYER) checkForPlayerTile();
            mover->getGridTile()->occupy(mover->getTile());
            mover->getTile()->stopDragging();
            mover->releaseTile();
        }
        else
        {
            this->removeItem(mover->getTile());
            delete mover->getTile();
            mover->releaseTile();
        }
    }
}
