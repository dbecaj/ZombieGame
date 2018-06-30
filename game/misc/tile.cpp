#include "tile.h"

#include "game/game.h"
#include "core/window.h"

extern Window* w;
Tile::Tile(QPixmap image, QPointF position, TileType tileType)
{
    this->setPixmap(image);
    this->setPos(position);
    this->position = position;
    this->tileType = tileType;
}

QPointF Tile::getCenterPosition()
{
    return QPointF(position.x() + TILE_SIZE/2.0f,
                   position.y() + TILE_SIZE/2.0f);
}

QRectF Tile::boundingRect() const
{
    return QRectF(0.0f, 0.0f, TILE_SIZE, TILE_SIZE);
}

QPainterPath Tile::shape() const
{
    QPainterPath path;
    path.addRect(0.0f, 0.0f, TILE_SIZE, TILE_SIZE);

    return path;
}
