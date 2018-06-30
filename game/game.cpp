#include "game.h"

#include <QCloseEvent>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QBrush>
#include <QImage>
#include <random>
#include <ctime>
#include <QMouseEvent>
#include <QMessageBox>
#include <QCoreApplication>
#include <functional>
#include <QGraphicsSceneWheelEvent>

#include "core/inputmanager.h"
#include "entities/mobs/player.h"
#include "misc/tile.h"
#include "entities/mobs/scientist.h"
#include "entities/mobs/zombie.h"
#include "ui/score.h"
#include "core/window.h"
#include "main_menu/mainmenu.h"
#include "ui/pausemenu.h"
#include "entities/entity.h"
#include "misc/gun.h"
#include "ui/uimanager.h"
#include "misc/language.h"

extern Window* w;
extern bool debug;
Game::Game(QGraphicsView* parent, SceneType type) : Scene(type, parent)
{
    this->inputManager = w->getInputManager();
    gameLoopTimer = new QTimer(this);
    fpsTime = (1/FPS)*1000.0f;
    gameLoopTimer->setInterval(fpsTime);
    reset = false;
    score = nullptr;
    pauseMenu = nullptr;
    ui = nullptr;
}

void Game::init(QString levelPath)
{
    if(reset) cleanUp(); // If we ran this game already we do a clean up just in case
    if(!pauseMenu)
    {
        pauseMenu = new PauseMenu(this); // Lazy init because we are lazy :3
        pauseMenu->setZValue(0.01f);
    }

    if(!reset) this->connect(gameLoopTimer, SIGNAL(timeout()), this, SLOT(gameLoop())); // If we don't do that we connect could multiple times which throws out an error

    frameCounter = 0;
    gameLoopTimer->start();

    w->setBackgroundBrush(QBrush(QImage(":/images/textures/tile.png")));

    level.init(levelPath);
    level.loadLevel(this);

    init();
}

void Game::init()
{
    for(int i = 0; i < zombies.length(); i++)
        this->connect(zombies[i], SIGNAL(mobHit(Mob*)), this, SLOT(zombieMobHit(Mob*)));

    player = new Player(level.playerStartPos);
    Gun* gun = player->getCurrentGun();
    this->connect(gun, SIGNAL(mobHit(Mob*)), this, SLOT(playerMobHit(Mob*))); // connect the signal when a player hits a mob for handling
    this->addItem(player);

    ui = new  UIManager();
    this->addItem(ui);

    score = new Score();
    ui->addComponent(score);
    gameState = GameState::RUNNING;
    reset = true;
}

Game::~Game()
{
}


void Game::cleanUp()
{

    // Because we clean all the objects in the scene we don't need to delete them all separatly (player, scientists, zombies)
    for(QGraphicsItem* item : items())
        delete item;

    pauseMenu = nullptr;
    ui = nullptr;
    score = nullptr;
    // Just clear the pointers that point to deallocated memory
    scientists.clear();
    zombies.clear();
    tiles.clear();
}

void Game::quitGame()
{
    gameLoopTimer->stop();
    cleanUp();
    w->setCurrentScene(w->getMainMenu());
}

bool Game::gameLoop()
{
    gameLoopTimer->setInterval(fpsTime);
    QTime currentTime = QTime::currentTime().addMSecs(fpsTime);

    if(gameState != GameState::PAUSED) handleInput(); // If the game is paused the player can't move
    updateEntities();
    handleCollision();
    ui->update();
    this->update(this->sceneRect()); // We just update the whole scene (we might want to limit that to just the current view)
    frameCounter++;
    sortForDrawing();
    if(gameState != GameState::RUNNING && gameState != GameState::PAUSED) endGame();

    int ms = QTime::currentTime().msecsTo(currentTime);
    if(ms < 0) gameLoopTimer->setInterval(0);
}

void Game::handleInput()
{
    player->handleInput();
}

void Game::handleCollision()
{
    player->tileCollision();
    for(int i = 0; i < scientists.length(); i++)
        scientists[i]->tileCollision();
    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->tileCollision();

    player->entityCollision();
    for(int i = 0; i < scientists.length(); i++)
        scientists[i]->mobCollision();
    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->mobCollision();
}

void Game::updateEntities()
{
    for(int i = 0; i < scientists.length(); i++)
        scientists[i]->update();

    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->update();

    player->update();
}

void Game::sortForDrawing()
{
    QList<QGraphicsItem*> items = this->items();
    QList<QGraphicsItem*> mobs;
    for(int i = 0; i < items.length(); i++)
    {
        if(items[i]->type() == ObjectType::ENTITY)
            mobs.push_back(items[i]);
    }

    for(int i = 0; i < mobs.size(); i++)
    {
        for(int j = 0; j < (mobs.size()-i-1); j++)
        {
            if(mobs[j]->y() < mobs[j+1]->y())
            {
                QGraphicsItem* mob = mobs[j];
                mobs[j] = mobs[j+1];
                mobs[j+1] = mob;
            }
        }
    }

    float startingZValue = 0.001f;
    float zDecrement = 0.000001f;

    for(int i = 0; i < mobs.length(); i++)
    {
        mobs[i]->setZValue(startingZValue);
        startingZValue -= zDecrement;
    }
}

void Game::zombieMobHit(Mob *mob)
{
    if(mob->getMobType() == MobType::PLAYER)
    {
        gameState = GameState::LOSE;
        return;
    }
    else if(mob->getMobType() == MobType::SCIENTIST)
    {
        // Remove the hit scientist and spawn a new zombie on its position
        Zombie* newZombie = new Zombie(mob->pos());
        zombies.push_back(newZombie);
        this->connect(newZombie, SIGNAL(mobHit(Mob*)), this, SLOT(zombieMobHit(Mob*)));
        scientists.removeOne(dynamic_cast<Scientist*>(mob));
        delete mob;

        this->addItem(newZombie);
    }
}

void Game::playerMobHit(Mob *mob)
{
    if(mob->getMobType() != MobType::ZOMBIE) return;

    // We know that mob is a zombie
    Zombie* zombie = dynamic_cast<Zombie*>(mob);

    zombie->damage(1);
    if(zombie->getLives() != 0) return;

    zombies.removeOne(zombie);
    this->removeItem(zombie);
    zombie->deleteLater();
}

void Game::endGame()
{
    QGraphicsTextItem* endText = new QGraphicsTextItem;
    endText->setZValue(0.011f);
    QString text;
    if(gameState == GameState::WIN)
    {
        endText->setDefaultTextColor(Qt::green);
        text = QString(Language::get("you_won"));
    }
    else if(gameState == GameState::LOSE)
    {
        endText->setDefaultTextColor(Qt::red);
        text = QString(Language::get("you_lose"));
    }

    // Just set the end text in the middle of the screen
    endText->setFont(QFont("VCR OSD Mono", 42));
    endText->setPlainText(text);
    endText->setPos(w->mapToScene(w->getWindowWidth()/2.0f - ((text.length()*40)/2.0f), w->getWindowHeight()/2.0f - 20));
    this->addItem(endText);
    gameLoopTimer->stop();
}

QList<Mob*> Game::getMobs()
{
    QList<Mob*> mobs;
    mobs.push_back(player);
    for(int i = 0; i < zombies.length(); i++) mobs.push_back(zombies[i]);
    for(int i = 0; i < scientists.length(); i++) mobs.push_back(scientists[i]);

    return mobs;
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        if(gameState != GameState::PAUSED)
        {
            // Pause the game and show the pauseMenu in the middle of the screen near top
            gameState = GameState::PAUSED;
            QSize windowSize(500, 600);
            QPointF scenePos = w->mapToScene(w->getWindowWidth()/2 - windowSize.width()/2, 50);
            pauseMenu->setPos(scenePos);
            this->addItem(pauseMenu);
        }
        else
        {
            // Unpause the game
            gameState = GameState::RUNNING;
            this->removeItem(pauseMenu);
        }
    }
}

void Game::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->accept();
}
