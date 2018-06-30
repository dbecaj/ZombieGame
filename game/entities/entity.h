#ifndef ENTITY_H
#define ENTITY_H

#include <QPointF>
#include <QGraphicsItem>
#include "game/game.h"

enum class EntityType
{
    MOB,
    BULLET
};

class Entity : public QGraphicsItem
{
    EntityType entityType;
protected:
    QPixmap pixmap;
public:
    explicit Entity(QPointF position, EntityType entityType, QGraphicsItem* parent = nullptr);
    virtual ~Entity();

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE = 0;// This is used by QT drawing (paint) to know what area to paint also the pos() returns the coords of boundingRect
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual QPointF getCenterPosition() = 0; // ps: this is kinda silly :P
    virtual void update() = 0; // This is implemented differently for different entity types

    int type() const { return ObjectType::ENTITY; }
    EntityType getType() { return entityType; }
};

#endif // ENTITY_H
