#include "tileitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

TileItem::TileItem(QPixmap pixmap, QPointF pos, TileType type, QObject *parent) : QObject(parent)
{
    this->pixmap = pixmap;
    this->type = type;

    this->setPixmap(pixmap.scaled(TILE_ITEM_SIZE, TILE_ITEM_SIZE));
    this->setPos(pos);
}

QRectF TileItem::boundingRect() const
{
    return QRectF(0.0f, 0.0f, TILE_ITEM_SIZE, TILE_ITEM_SIZE);
}

void TileItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) emit clickEvent(this);
    QGraphicsItem::mousePressEvent(event);
}
