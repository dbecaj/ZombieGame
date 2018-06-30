#ifndef GRID_H
#define GRID_H

#include <QList>
#include <QPointF>
#include <QGraphicsItem>

class GridTile;

class Grid : public QGraphicsItem
{
    QList<GridTile*> tiles;
    int width;
    int height;

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public:
    explicit Grid(int width, int height, QGraphicsItem* parent = nullptr);
    ~Grid();

    QList<GridTile*> getTiles() { return tiles; }
    GridTile* getClosestTile(QPointF pos);
    QList<QString> getLevelData();
    GridTile* getPlayerTile();
};

#endif // GRID_H
