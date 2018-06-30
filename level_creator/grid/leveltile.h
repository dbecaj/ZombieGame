#ifndef LEVELTILE_H
#define LEVELTILE_H

#include "level_creator/ui/tileitem.h"

class LevelTile : public TileItem
{
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
public:
    LevelTile(TileItem* item, QPointF pos);

    void stopDragging();

    enum class TileState
    {
        RED,
        GREEN,
        PLACED
    };

private:
    TileState state;
public:
    TileState getState() { return state; }
    void setState(TileState state) { this->state = state; }
};

#endif // LEVELTILE_H
