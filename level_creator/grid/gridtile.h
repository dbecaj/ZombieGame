#ifndef GRIDTILE_H
#define GRIDTILE_H

#include <QPointF>

class LevelTile;

class GridTile
{
    QPointF pos;
    LevelTile* tile;
public:
    GridTile(QPointF pos);

    QPointF getPos() { return pos; }
    bool isOccupied();
    void occupy(LevelTile* tile);
    void unoccupy();
    LevelTile* getTile() { return tile; }
};

#endif // GRIDTILE_H
