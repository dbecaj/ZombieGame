#include "leveltile.h"

#include <QPainter>
#include <QDebug>

#include "core/window.h"
#include "level_creator/levelcreator.h"
#include "level_creator/grid/grid.h"
#include "level_creator/grid/gridtile.h"
#include "level_creator/ui/bottombar.h"

extern Window* w;
LevelTile::LevelTile(TileItem* item, QPointF pos) : TileItem(item->getPixmap(), pos, item->getType())
{
    this->setAcceptHoverEvents(true);
}

void LevelTile::stopDragging()
{
    state = TileState::PLACED;
}

void LevelTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(state == TileState::PLACED) painter->drawPixmap(0, 0, TILE_ITEM_SIZE, TILE_ITEM_SIZE, pixmap);
    else
    {
        painter->setOpacity(0.7f);
        painter->drawPixmap(0, 0, TILE_ITEM_SIZE, TILE_ITEM_SIZE, pixmap);
        painter->setOpacity(1.0f);
        if(state == TileState::RED) painter->setBrush(QBrush(QColor(200, 0, 0, 125)));
        else if(state == TileState::GREEN) painter->setBrush(QBrush(QColor(0, 200, 0, 125)));
        painter->drawRect(0, 0, TILE_ITEM_SIZE, TILE_ITEM_SIZE);
    }
}

void LevelTile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(state == TileState::RED)
    {
        QList<GridTile*> tiles = w->getLevelCreator()->getGrid()->getTiles();
        for(int i = 0; i < tiles.length(); i++)
        {
            if(tiles[i]->getTile() == this)
            {
                tiles[i]->unoccupy();
                w->setCursor(QCursor(w->getLevelCreator()->getBottomBar()->getRemovePixmap()));
                w->getLevelCreator()->setRemoveMode(true);
                this->deleteLater();
            }
        }
    }
}

void LevelTile::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(w->getLevelCreator()->isInRemoveMode()) state = TileState::RED;
}

void LevelTile::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(w->getLevelCreator()->isInRemoveMode()) state = TileState::PLACED;
}
