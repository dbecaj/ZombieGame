#include "playermp.h"

#include <QDebug>

#include "game/misc/gun.h"

PlayerMP::PlayerMP(QPointF position, QString username) : Player(position)
{
    this->username = username;
    this->enableNameTag(username); // Automatically enable name tag
}

void PlayerMP::updatePosition(QPointF position, float degrees)
{
    this->degrees = degrees;
    this->position = position;
    isMoving = false;
    if(this->pos() != position)
    {
        isMoving = true; // If the new position is not the same as current position(we send player pos every 1/60.0sec)
        shooting = false;
    }
}

void PlayerMP::fireBullet()
{
    this->getCurrentGun()->fire(true); // Ignore gun cooldown
    shooting = true;
    if(shooting) resetAnim = true;
}

void PlayerMP::update() // we only update if the updatePosition,fireBullet has been called
{
    this->setPos(position);
    this->setPlayerWalkState();
    this->getCurrentGun()->update();
}
