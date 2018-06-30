#include "scientistmp.h"

#include <qmath.h>

ScientistMP::ScientistMP(ScientistGender gender, QPointF position, long id) : Scientist(gender, position, id)
{

}

void ScientistMP::update()
{
    float rad = qDegreesToRadians(angle);
    this->setX(this->x() + qCos(rad) * speed);
    this->setY(this->y() + qSin(rad) * speed);

    if(angle >= 45.0f && angle <= 135.0f) walkState = MobWalkState::WALK_FORWARD;
    else if(angle >= 135.0f && angle <= 225.0f) walkState = MobWalkState::WALK_LEFT;
    else if(angle >= 225.0f && angle <= 315.0f) walkState = MobWalkState::WALK_BACKWARD;
    else walkState = MobWalkState::WALK_RIGHT;
}

void ScientistMP::updatePos(float x, float y, float dir)
{
    angle = dir;
    this->setPos(QPointF(x, y));

    if(angle >= 45.0f && angle <= 135.0f) walkState = MobWalkState::WALK_FORWARD;
    else if(angle >= 135.0f && angle <= 225.0f) walkState = MobWalkState::WALK_LEFT;
    else if(angle >= 225.0f && angle <= 315.0f) walkState = MobWalkState::WALK_BACKWARD;
    else walkState = MobWalkState::WALK_RIGHT;
}

void ScientistMP::changeAngle(float angle)
{
    this->angle = angle;
}
