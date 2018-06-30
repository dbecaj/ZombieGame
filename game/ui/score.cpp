#include "score.h"

#include <QTimer>
#include <QDebug>
#include <QGraphicsTextItem>

#include "game/game.h"
#include "core/window.h"
#include "uimanager.h"
#include "misc/language.h"

extern Window* w;
Score::Score()
{
    this->setPos(0, 0);

    scientistText = new QGraphicsTextItem();
    zombieText = new QGraphicsTextItem();

    scientistText->setDefaultTextColor(Qt::white);
    scientistText->setFont(QFont("times", 16));
    scientistText->setPos(0, 0);

    zombieText->setDefaultTextColor(Qt::green);
    zombieText->setFont(QFont("times", 16));
    zombieText->setPos(0, 20);

    this->addToGroup(scientistText);
    this->addToGroup(zombieText);
}

void Score::update()
{
    scientistText->setPlainText(QString(Language::get("scientists") + ": ") + QString::number(w->getGame()->scientists.size()));
    zombieText->setPlainText(QString(Language::get("zombies") + ": ") + QString::number(w->getGame()->zombies.size()));
    // Check if we won (the score decides that)
    if(w->getGame()->zombies.size() == 0) w->getGame()->gameState = GameState::WIN;
}
