#include "mob.h"

#include <QVector2D>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

#include "game/misc/tile.h"

extern bool debug;
Mob::Mob(QPointF position, MobType mobType) : Entity(position, EntityType::MOB)
{
    this->mobType = mobType;
    colliding = false;
}

Mob::~Mob()
{
}

QRectF Mob::boundingRect() const
{
    // Position of the QRectF is relative to pos() of the QGraphicsItem so we set it at 0.0f,0.0f (so if the pos()->x() == 500.0f the QRectF
    // position is also 500.0f
    return QRectF(0.0f, 0.0f, MOB_WIDTH, MOB_HEIGHT);
}

QPainterPath Mob::shape() const
{
    // Similar here we just move the y coord down the boundingRect() box so the collision box is at his feet
    QPainterPath path;
    path.addRect(0.0f, MOB_HEIGHT - MOB_COLL_HEIGHT, MOB_WIDTH, MOB_COLL_HEIGHT);
    return path;
}

void Mob::update()
{
}

void Mob::tileCollision()
{
    colliding = false;
    // Here we say that the minimum distance should be atleast half of the mob x,y and half of the tile x,y
    const static float MIN_DIST_X = MOB_WIDTH/2.0f + TILE_SIZE/2.0f;
    const static float MIN_DIST_Y = MOB_COLL_HEIGHT/2.0f + TILE_SIZE/2.0f;

    QList<QGraphicsItem*> collidingObjects = this->collidingItems();
    for(int i = 0; i < collidingObjects.size(); i++)
    {
        // If we collide with an object that has type() TILE which we overloaded in our QGraphicsItem objects we cast QGraphicsItem to Tile
        if(collidingObjects[i]->type() != ObjectType::TILE) continue;
        Tile* tile = (Tile*)collidingObjects[i];

        colliding = true;

        // We temporary store current x and y for convenience
        float x = this->pos().x();
        float y = this->pos().y();
        QPointF center(getCenterPosition());
        QPointF tileCenter(tile->getCenterPosition());
        // Calculate the distance between two centers (center.x - tileCenter.x, center.y - tileCenter.y)
        QPointF distance = center - tileCenter;

        // Calculate the collision depth for the x and y
        float xDepth = MIN_DIST_X - qAbs(distance.x());
        float yDepth = MIN_DIST_Y - qAbs(distance.y());

        // Find out which collision is shallower
        if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
        {
             // If it's x we correct the x coord
             if (distance.x() < 0.0f) this->setPos(x-xDepth, y);
             else this->setPos(x+xDepth, y);
        }
        else
        {
            // If it's y we correct the y coord
            if (distance.y() < 0.0f) this->setPos(x, y-yDepth);
            else this->setPos(x, y+yDepth);
        }
    }
}

void Mob::mobCollision()
{
    // Since all mobs are the same size the minimal distance is half of the mob x,y + half of the mob x,y == MOB_WIDTH and MOB_HEIGHT
    const static float MIN_DIST_X = MOB_WIDTH;
    const static float MIN_DIST_Y = MOB_COLL_HEIGHT;
    QList<QGraphicsItem*> collidingObjects = this->collidingItems();
    for(int i = 0; i < collidingObjects.size(); i++)
    {
        // Check if object is ENTITY if it is cast it to Entity
        if(collidingObjects[i]->type() != ObjectType::ENTITY) continue;
        Entity* entity = (Entity*)collidingObjects[i];

        float x = this->pos().x();
        float y = this->pos().y();
        // Get the centers and store them in a QVector2D because we need the normalize() (direction of the vector)
        QVector2D center(getCenterPosition().x(), getCenterPosition().y());
        QVector2D entityCenter(entity->getCenterPosition().x(), entity->getCenterPosition().y());
        // Calculate the distance between two centers (center.x - tileCenter.x, center.y - tileCenter.y)
        QVector2D distance = center - entityCenter;

        // Calculate the collision depth for the x and y
        float xDepth = MIN_DIST_X - abs(distance.x());
        float yDepth = MIN_DIST_Y - abs(distance.y());

        // Then we divide the depths by 2.0f so that we can move both mobs by half the depth
        xDepth /= 2.0f;
        yDepth /= 2.0f;

        // Similar collision to tile collision with an exeption that we also move the mob
        if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
        {
            if (distance.x() < 0.0f)
            {
                this->setPos(x-xDepth, y);

                x = entity->pos().x();
                y = entity->pos().y();
                entity->setPos(x+xDepth, y);
            }
            else
            {
                this->setPos(x+xDepth, y);

                x = entity->pos().x();
                y = entity->pos().y();
                entity->setPos(x-xDepth, y);
            }
        }
        else
        {
            if (distance.y() < 0.0f)
            {
                this->setPos(x, y-yDepth);

                x = entity->pos().x();
                y = entity->pos().y();
                entity->setPos(x, y+yDepth);
            }
            else
            {
                this->setPos(x, y+yDepth);

                x = entity->pos().x();
                y = entity->pos().y();
                entity->setPos(x, y-yDepth);
            }
         }
    }
}

void Mob::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // If the debug is true we draw a red rect around the boundingRect and green rect around the shape aka. collision box
    if(!debug) return;
    QPen pen(Qt::red, 1);
    painter->setPen(pen);
    painter->drawRect(boundingRect());
    pen.setColor(Qt::green);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(0.0f, MOB_HEIGHT - MOB_COLL_HEIGHT, MOB_WIDTH, MOB_COLL_HEIGHT);
}

QPointF Mob::getCenterPosition()
{
    // The only special thing here is the this->pos()->y() is from the boundingRect pos so we need to take that into consideration
    return QPoint(this->pos().x() + MOB_WIDTH/2.0f,
                  this->pos().y() + (MOB_HEIGHT - MOB_COLL_HEIGHT) + MOB_COLL_HEIGHT/2.0f);
}
