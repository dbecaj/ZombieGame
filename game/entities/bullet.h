#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QPointF>

#define BULLET_SIZE 10.0f

enum Direction
{
    UP = 0,
    UP_LEFT = 1,
    UP_RIGHT = 2,
    DOWN = 3,
    DOWN_LEFT = 4,
    DOWN_RIGHT = 5,
    LEFT = 6,
    RIGHT = 7
};

class Bullet : public QGraphicsPixmapItem
{
    Direction direction;
    QPointF position;
    float speed;
public:
    Bullet(QPointF position, Direction direction);

    void update(); // This just moves the bullet in the set direction ps: Would be better if we had a float angle not just 8 directions

    Direction getDirection() { return direction; }
    QPointF getPosition() { return position; }

};

#endif // BULLET_H
