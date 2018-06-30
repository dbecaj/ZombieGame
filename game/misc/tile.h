#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QPixmap>
#include <QPainterPath>

#include "game/entities/entity.h"

#define TILE_SIZE 100.0f

enum class TileType
{
    WALL,
    BOX,
    COMPUTER,
    BOOKSHELF
};

class Tile : public QGraphicsPixmapItem
{
    QPointF position;
    TileType tileType;

    QRectF boundingRect() const;
    QPainterPath shape() const;
public:
    Tile(QPixmap image, QPointF position, TileType tileType);

    int type() const { return ObjectType::TILE; }

    QPointF getCenterPosition();
    TileType getTileType() { return tileType; }
};

#endif // TILE_H
