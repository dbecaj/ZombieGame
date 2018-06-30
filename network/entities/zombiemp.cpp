#include "zombiemp.h"

#include <QDebug>

#include "core/window.h"
#include "game/multiplayergame.h"

extern Window* w;
ZombieMP::ZombieMP(QPointF position, long id) : Zombie(position, id)
{
    this->target = nullptr;
}

void ZombieMP::update()
{
    if(!target) return;

    QVector2D zombieCenter(this->getCenterPosition().x(), this->getCenterPosition().y());
    QVector2D targetCenter(target->getCenterPosition().x(), target->getCenterPosition().y());
    QVector2D targetLocation = zombieCenter - targetCenter;

    this->setPos(this->x() - (targetLocation.normalized().x() * speed), this->y() - (targetLocation.normalized().y() * speed));
    angle = std::atan2(targetLocation.normalized().x(), targetLocation.normalized().y()) * (180.0f/M_PI);

    this->setZombieWalkState();
}

void ZombieMP::setTarget(Mob *target)
{
    this->target = target;
}

void ZombieMP::mobCollision()
{
    Mob::mobCollision();
}
