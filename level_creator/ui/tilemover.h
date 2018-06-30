#ifndef TILEMOVER_H
#define TILEMOVER_H

class LevelTile;
class GridTile;

class TileMover
{
    LevelTile* tile;
    GridTile* gridTile;
public:
    TileMover();

    void setTile(LevelTile *item);
    LevelTile* getTile() { return tile; }
    GridTile* getGridTile() { return gridTile; }
    bool releaseTile();
    bool hasTile();

    void update();
};

#endif // TILEMOVER_H
