#include "gridtile.h"

#include "leveltile.h"

GridTile::GridTile(QPointF pos)
{
    this->pos = pos;
    tile = nullptr;
}

bool GridTile::isOccupied()
{
    if(tile) return true;
    return false;
}

void GridTile::occupy(LevelTile* tile)
{
    this->tile = tile;
}

void GridTile::unoccupy()
{
    tile = nullptr;
}
