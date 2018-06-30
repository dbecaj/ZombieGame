#ifndef MOB_H
#define MOB_H

#include <QGraphicsItem>

#include "game/entities/entity.h"

#define MOB_WIDTH 100.0f * 0.574468f
#define MOB_HEIGHT 100.0f
#define MOB_COLL_HEIGHT 30.0f

enum class MobWalkState
{
    WALK_FORWARD,
    WALK_BACKWARD,
    WALK_LEFT,
    WALK_RIGHT,
};

enum class MobType
{
    PLAYER,
    SCIENTIST,
    ZOMBIE
};

class Mob : public Entity
{
protected:
    MobWalkState walkState;
    MobType mobType;
    bool colliding; // This is set to true when we are colliding with a Tile
public:
    Mob(QPointF position, MobType mobType);
    virtual ~Mob();

    QRectF boundingRect() const Q_DECL_OVERRIDE; // This is used by QT drawing (paint) to know what area to paint also the pos() returns the coords of boundingRect
    QPainterPath shape() const Q_DECL_OVERRIDE; /*This is used when QT is checking the collision with other QGraphicsObjects and we mainly use the position from here
                                                  by converting it from boundingRect() position*/
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void tileCollision(); // Handles the collision with all the tiles
    virtual void mobCollision(); // Handles the collision with all the entities
    virtual void update(); // This is used for updating the mob and is different for every mob (here it's empty)

    QPointF getCenterPosition(); //This returns the collision box center
    MobType getMobType() { return mobType; }
};

#endif // MOB_H
