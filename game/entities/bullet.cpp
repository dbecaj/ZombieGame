#include "bullet.h"

#include <typeinfo>
#include <qmath.h>

#include "game/game.h"
#include "core/window.h"
#include "game/misc/tile.h"
#include "mobs/scientist.h"
#include "mobs/zombie.h"

extern Window* w;
Bullet::Bullet(QPointF position, Direction direction)
{
    this->direction = direction;
    speed = 15.0f;
    this->position = position;
    this->setPixmap(QPixmap(":/images/textures/bullet.png").scaled(10, 10));
    this->setPos(position);
}

void Bullet::update()
{
    float x = this->x();
    float y = this->y();
    switch(direction)
    {
        case Direction::UP:
        {
            this->setPos(x, y - speed);
            break;
        }
        case Direction::UP_LEFT:
        {
            this->setPos(x - speed, y - speed);
            break;
        }
        case Direction::UP_RIGHT:
        {
            this->setPos(x + speed, y - speed);
            break;
        }
        case Direction::DOWN:
        {
            this->setPos(x, y + speed);
            break;
        }
        case Direction::DOWN_LEFT:
        {
            this->setPos(x - speed, y + speed);
            break;
        }
        case Direction::DOWN_RIGHT:
        {
            this->setPos(x + speed, y + speed);
            break;
        }
        case Direction::LEFT:
        {
            this->setPos(x - speed, y);
            break;
        }
        case Direction::RIGHT:
        {
            this->setPos(x + speed, y);
            break;
        }
    }

    position = this->pos();
}
