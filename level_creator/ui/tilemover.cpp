#include "tilemover.h"

#include <QVector2D>
#include <QDebug>

#include "core/inputmanager.h"
#include "core/window.h"
#include "tileitem.h"
#include "level_creator/grid/gridtile.h"
#include "level_creator/levelcreator.h"
#include "level_creator/grid/grid.h"
#include "level_creator/grid/leveltile.h"

extern Window* w;
TileMover::TileMover()
{
    tile = nullptr;
    gridTile = nullptr;
}

void TileMover::setTile(LevelTile *tile)
{
    this->tile = tile;
    w->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));
}

bool TileMover::releaseTile()
{
    if(tile)
    {
        tile = nullptr;
        w->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        return true;
    }

    return false;
}

bool TileMover::hasTile()
{
    if(tile) return true;
    return false;
}

void TileMover::update()
{
    if(!tile) return;

    QPointF mousePos = w->mapToScene(w->getInputManager()->mousePosition.toPoint());
    GridTile* gridTile = w->getLevelCreator()->getGrid()->getClosestTile(mousePos);
    QVector2D distance = QVector2D(mousePos) - QVector2D(gridTile->getPos());
    if(distance.length() < 20.0f && !gridTile->isOccupied())
    {
        tile->setPos(gridTile->getPos().x() - 50.0f, gridTile->getPos().y() - 50.0f);
        tile->setState(LevelTile::TileState::GREEN);
        this->gridTile = gridTile;
    }
    else
    {
        tile->setState(LevelTile::TileState::RED);
        tile->setPos(mousePos.x() - (TILE_ITEM_SIZE/2.0f),
                     mousePos.y() - (TILE_ITEM_SIZE/2.0f));
    }
}
