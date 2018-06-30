#include "zombie.h"

#include <QDebug>
#include <QLineF>
#include <qmath.h>
#include <typeinfo>
#include <QMutableListIterator>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <random>
#include <ctime>

#include "game/game.h"
#include "core/window.h"
#include "player.h"
#include "scientist.h"
#include "game/misc/tile.h"
#include "game/multiplayergame.h"

extern Window* w;

QSoundEffect Zombie::attackedSound;

Zombie::Zombie(QPointF position, long id) : Mob(position, MobType::ZOMBIE)
{
    // FRAME_TIME tells us how many game frames does each animation frame last
    static const float FRAME_TIME = 7.0f;

    // Load zombie sprite map
    QString path = ":/images/textures/zombie_sprite.png";
    if(!pixmap.load(path)) qDebug() << "Zombie>failed to load texture with path: " << path;

    lives = 3;
    speed = 7.0f;
    // Initialize all animFWs
    animFW[0] = new AnimationFramework(FRAME_TIME, 6); // WALK_FORWARD animation
    animFW[1] = new AnimationFramework(FRAME_TIME, 4); // WALK_BACKWARD
    animFW[2] = new AnimationFramework(FRAME_TIME, 3); // WALK_LEFT
    animFW[3] = new AnimationFramework(FRAME_TIME, 3); // WALK_RIGHT
    target = nullptr;

    // id default is -1
    if(id >= 0) this->id = id;
    else this->id = idIncrement++;

    attackedSound.setSource(QUrl("qrc:/sound/sound/zombie_attacked.wav"));
    attackedSound.setVolume(0.2f);
}

long Zombie::idIncrement = 0;

Zombie::~Zombie()
{
    // Delete all objects that are not covered by the QT framework
    for(int i = 0; i < 4; i++)
        delete animFW[i];
}

void Zombie::update()
{
    target = scanForTarget();
    if(!target) return; // This will realy never happen because player will always be in a normal game but better save then sorry

    QVector2D zombieCenter(this->getCenterPosition().x(), this->getCenterPosition().y());
    QVector2D targetCenter(target->getCenterPosition().x(), target->getCenterPosition().y());
    QVector2D targetLocation = zombieCenter - targetCenter;

    // Move the zombie in the direction of the target
    this->setPos(this->x() - (targetLocation.normalized().x() * speed), this->y() - (targetLocation.normalized().y() * speed));
    angle = std::atan2(targetLocation.normalized().x(), targetLocation.normalized().y()) * (180.0f/M_PI);
    setZombieWalkState();
}

void Zombie::setZombieWalkState()
{
    if(angle >= -45.0f && angle <= 45.0f) walkState = MobWalkState::WALK_BACKWARD;
    else if(angle >= 45.0f && angle <= 135.0f) walkState = MobWalkState::WALK_LEFT;
    else if(angle >= -135.0f && angle <= 135.0f) walkState = MobWalkState::WALK_RIGHT;
    else walkState = MobWalkState::WALK_FORWARD;
}

Mob* Zombie::scanForTarget()
{
    Mob* mob = nullptr;
    Game* game = (Game*)w->getCurrentScene();
    mob = game->player;
    if(!mob)
    {
        if(game->getScientists().length() == 0) return nullptr;
        mob = game->getScientists().first();
    }

    QVector2D zombieCenter(getCenterPosition().x(), getCenterPosition().y());
    QVector2D targetCenter(mob->getCenterPosition().x(), mob->getCenterPosition().y());

    for(int i = 0; i < w->getGame()->getScientists().length(); i++)
    {
        Scientist* sci = w->getGame()->getScientists()[i];
        QVector2D scientistCenter(sci->getCenterPosition().x(), sci->getCenterPosition().y());
        if((zombieCenter - targetCenter).length() > (zombieCenter - scientistCenter).length())
        {
            mob = sci;
            targetCenter = QVector2D(mob->getCenterPosition().x(), mob->getCenterPosition().y());
        }
    }

    // Since there are multiple players in MultiplayerGame we also scan those
    if(w->getCurrentScene()->getSceneType() != SceneType::MULTIPLAYER_GAME) return mob;

    for(int i = 0; i < w->getMultiplayerGame()->getPlayers().length(); i++)
    {
        PlayerMP* p = w->getMultiplayerGame()->getPlayers()[i];
        QVector2D playerCenter(p->getCenterPosition().x(), p->getCenterPosition().y());
        if((zombieCenter - targetCenter).length() > (zombieCenter - playerCenter).length())
        {
            mob = p;
            targetCenter = QVector2D(mob->getCenterPosition().x(), mob->getCenterPosition().y());
        }
    }

    return mob;
}

void Zombie::damage(int ammount)
{
    lives -= ammount;
    if(lives < 0) lives = 0;
    else playAttackedSound();
}

void Zombie::playAttackedSound()
{
    attackedSound.play();
}

void Zombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    static const QPointF dims(27.0f, 47.0f); // Zombie sprite dimensions
    Mob::paint(painter, option, widget); // draws rect around boundingRect() and shape() if the debug is true

    QRectF targetRect, sourceRect; // targetRect is where we will paint (boundingRect()) and sourceRect is the sprite that we will draw from the sprite map
    targetRect = this->boundingRect();
    switch(walkState)
    {
        case MobWalkState::WALK_FORWARD:
        {
            static const float X = 0.0f;
            static const float X2 = 51.0f;

            int animNum = animFW[0]->getAnimationNum();
            if(animNum == 1) sourceRect.setRect(X, 0.0f, dims.x(), dims.y());
            else if(animNum == 2) sourceRect.setRect(X, 51.0f, dims.x(), dims.y());
            else if(animNum == 3) sourceRect.setRect(X, 101.0f, dims.x(), dims.y());
            else if(animNum == 4) sourceRect.setRect(X2, 0.0f, dims.x(), dims.y());
            else if(animNum == 5) sourceRect.setRect(X2, 51.0f, dims.x(), dims.y());
            else sourceRect.setRect(X2, 101.0f, dims.x(), dims.y());

            // We update the animation framework on that index so that the animation flows and the animationNum changes every NUM_OF_FRAMES
            animFW[0]->update();
            break;
        }
        case MobWalkState::WALK_BACKWARD:
        {
            static const float X = 102.0f;
            static const float X2 = 153.0f;

            int animNum = animFW[1]->getAnimationNum();
            if(animNum == 1) sourceRect.setRect(X, 0.0f, dims.x(), dims.y());
            else if(animNum == 2) sourceRect.setRect(X2, 0.0f, dims.x(), dims.y());
            else if(animNum == 3) sourceRect.setRect(X, 51.0f, dims.x(), dims.y());
            else sourceRect.setRect(X2, 51.0f, dims.x(), dims.y());

            animFW[1]->update();
            break;
        }
        case MobWalkState::WALK_LEFT:
        {
            static const float X = 201.0f;

            int animNum = animFW[2]->getAnimationNum();
            if(animNum == 1) sourceRect.setRect(X, 0.0f, dims.x(), dims.y());
            else if(animNum == 2) sourceRect.setRect(X, 51.0f, dims.x(), dims.y());
            else sourceRect.setRect(X, 101.0f, dims.x(), dims.y());

            animFW[2]->update();
            break;
        }
        case MobWalkState::WALK_RIGHT:
        {
            static const float X = 250.0f;

            int animNum = animFW[3]->getAnimationNum();
            if(animNum == 1) sourceRect.setRect(X, 0.0f, dims.x(), dims.y());
            else if(animNum == 2) sourceRect.setRect(X, 51.0f, dims.x(), dims.y());
            else sourceRect.setRect(X, 101.0f, dims.x(), dims.y());

            animFW[3]->update();
            break;
        }
        default:
        {
            static const float X = 0.0f;
            static const float X2 = 51.0f;

            int animNum = animFW[0]->getAnimationNum();
            if(animNum == 1) sourceRect.setRect(X, 0.0f, dims.x(), dims.y());
            else if(animNum == 2) sourceRect.setRect(X, 51.0f, dims.x(), dims.y());
            else if(animNum == 3) sourceRect.setRect(X, 101.0f, dims.x(), dims.y());
            else if(animNum == 4) sourceRect.setRect(X2, 0.0f, dims.x(), dims.y());
            else if(animNum == 5) sourceRect.setRect(X2, 51.0f, dims.x(), dims.y());
            else sourceRect.setRect(X2, 101.0f, dims.x(), dims.y());

            // We update the animation framework on that index so that the animation flows and the animationNum changes every NUM_OF_FRAMES
            animFW[0]->update();
            break;
        }
    }
    // We just draw a correct sprite of the zombie (sourceRect) in the boundingBox(targetRect)
    painter->drawPixmap(targetRect, pixmap, sourceRect);
}

void Zombie::mobCollision()
{
    QList<QGraphicsItem*>collidingObjects = this->collidingItems();
    for(int i = 0; i < collidingObjects.size(); i++)
    {
        if(collidingObjects[i]->type() == ObjectType::ENTITY) // ps: Improve this all mobs are Entities but bullet isnt -.----
        {
            Mob* mob = dynamic_cast<Mob*>(collidingObjects[i]); // You are playing with fire Billy boy
            if(mob->getMobType() == MobType::PLAYER || mob->getMobType() == MobType::SCIENTIST) emit mobHit(mob);
        }
    }

    Mob::mobCollision(); // Now we just take them appart aka. do the collision
}
