#include "pausemenu.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>

#include "core/window.h"
#include "main_menu/button.h"
#include "main_menu/mainmenu.h"
#include "game/game.h"
#include "game/multiplayergame.h"
#include "misc/language.h"
#include "misc/musicmanager.h"

extern Window* w;
PauseMenu::PauseMenu(QObject *parent) : QObject(parent)
{
    // The PauseMenu consists of background QGraphicsPixmapItem and Buttons
    this->setHandlesChildEvents(false);
    QSize windowSize(500, 600);
    QPointF scenePos = w->mapFromScene(w->getWindowWidth()/2 - windowSize.width()/2, 50);
    this->setPos(scenePos.x(), scenePos.y());
    background = new QGraphicsPixmapItem(this);
    background->setPos(0, 0);
    background->setPixmap(QPixmap(":/images/textures/pause_menu_background.png").scaled(windowSize.width(), windowSize.height()));
    this->addToGroup(background);

    buttonSize = QSize(400, 100);

    QPixmap idleButton = QPixmap(":/images/textures/pause_button_idle.png");
    QPixmap activeButton = QPixmap(":/images/textures/pause_button_active.png");

    mainMenuButton = new Button(windowSize.width()/2 - buttonSize.width()/2, 40.0f, buttonSize.width(), buttonSize.height(), idleButton, activeButton,
                                Text(Language::get("main_menu"), QColor(255, 255, 255), QFont("impact", 42)), this);
    this->connect(mainMenuButton, SIGNAL(clickEvent()), this, SLOT(clickMainMenuButton()));
    lastY = mainMenuButton->pos().y() + buttonSize.height();
    buttonX = mainMenuButton->x();
}

void PauseMenu::addButton(Button *button, QObject* maker, const char* slot)
{
    this->addToGroup(button);
    button->setX(buttonX);
    button->setY(lastY + 50.0f);
    lastY = button->pos().y() + buttonSize.height();
    this->connect(button, SIGNAL(clickEvent()), maker, slot);
}

void PauseMenu::clickMainMenuButton()
{
    qDebug() << "click MainMenu";
    if(w->getCurrentScene()->getSceneType() == SceneType::MULTIPLAYER_GAME ||
       w->getCurrentScene()->getSceneType() == SceneType::GAME)
    {
        Game* game = (Game*)w->getCurrentScene();
        game->quitGame();
    }
    else
    {
        w->getCurrentScene()->cleanUp();
        w->setCurrentScene(w->getMainMenu());
    }
    MusicManager::play(MusicType::MAIN_MENU);
}
