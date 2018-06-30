#include "mainmenu.h"

#include <QDebug>
#include <QGraphicsSceneWheelEvent>

#include "core/window.h"
#include "core/inputmanager.h"
#include "button.h"
#include "game/game.h"
#include "multiplayerpage.h"
#include "level_creator/levelcreator.h"
#include "level_creator/levelsizepage.h"
#include "levelselectorpage.h"
#include "misc/language.h"
#include "misc/notification.h"
#include "misc/musicmanager.h"

extern Window* w;
MainMenu::MainMenu(QGraphicsView *parent) : Scene(SceneType::MAIN_MENU, parent)
{
    inputManager = w->getInputManager();
    multiplayerPage = new MultiplayerPage(this);
    levelSizePage = new LevelSizePage(this);
    levelPage = new LevelSelectorPage(this);
}

void MainMenu::init()
{
    this->setSceneRect(0, 0, w->getWindowWidth(), w->getWindowHeight());

    QSizeF buttonSize(500.0f * w->getWidthScale(), 200.0f * w->getHeightScale());
    float middleX = w->getWindowWidth()/2.0f - buttonSize.width()/2.0f;
    float middleY = 50.0f;

    QPixmap menuIdle = QPixmap(":/images/textures/menu_button_idle.png");
    QPixmap menuActive = QPixmap(":/images/textures/menu_button_active.png");
    QColor textColorActive(255, 165, 0);
    // We add all our mainMenu buttons and connect them to slots for handling
    playButton = new Button(middleX, middleY, buttonSize.width(), buttonSize.height(), menuIdle, menuActive,
                            Text(Language::get("play"), QColor(255, 255, 255), QFont("VCR OSD Mono", 42)));
    playButton->setActiveTextColor(textColorActive);
    this->connect(playButton, SIGNAL(clickEvent()), this, SLOT(clickPlayButton()));
    this->addItem(playButton);
    middleY += buttonSize.height() + 25.0f;

    createButton = new Button(middleX, middleY, buttonSize.width(), buttonSize.height(), menuIdle, menuActive,
                              Text(Language::get("create"), QColor(255, 255, 255), QFont("VCR OSD Mono", 42)));
    createButton->setActiveTextColor(textColorActive);
    this->connect(createButton, SIGNAL(clickEvent()), this, SLOT(clickCreateButton()));
    this->addItem(createButton);
    middleY += buttonSize.height() + 25.0f;

    multiplayerButton = new Button(middleX, middleY, buttonSize.width(), buttonSize.height(), menuIdle, menuActive,
                                   Text(Language::get("multiplayer"), QColor(255, 255, 255), QFont("VCR OSD Mono", 26)));
    multiplayerButton->setActiveTextColor(textColorActive);
    this->connect(multiplayerButton, SIGNAL(clickEvent()), this, SLOT(clickMultiplayerButton()));
    this->addItem(multiplayerButton);
    middleY += buttonSize.height() + 25.0f;

    quitButton = new Button(middleX, middleY, buttonSize.width(), buttonSize.height(), menuIdle, menuActive,
                            Text(Language::get("quit"), QColor(255, 255, 255), QFont("VCR OSD Mono", 42)));
    quitButton->setActiveTextColor(textColorActive);
    this->connect(quitButton, SIGNAL(clickEvent()), this, SLOT(clickQuitButton()));
    this->addItem(quitButton);

    MusicManager::play(MusicType::MAIN_MENU);
}

void MainMenu::cleanUp()
{
}

void MainMenu::clickPlayButton()
{
    levelPage->init();
    w->setCurrentScene(levelPage);
}

void MainMenu::clickCreateButton()
{
    levelSizePage->init();
    w->setCurrentScene(levelSizePage);
}

void MainMenu::clickMultiplayerButton()
{
    multiplayerPage->init();
    multiplayerPage->resetContent();
    w->setCurrentScene(multiplayerPage);
}

void MainMenu::clickQuitButton()
{
    w->close();
}

void MainMenu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void MainMenu::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void MainMenu::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->accept();
    QGraphicsScene::wheelEvent(event);
}
