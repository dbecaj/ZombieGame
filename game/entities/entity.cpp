#include "entity.h"

#include <qmath.h>
#include <QVector2D>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QPainter>

#include "game/misc/tile.h"
#include "game/game.h"

extern bool debug;
Entity::Entity(QPointF position, EntityType entityType, QGraphicsItem* parent) : QGraphicsItem(parent)
{
    this->setPos(position);
    this->entityType = entityType;
}

Entity::~Entity()
{
}
