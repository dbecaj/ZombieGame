#include "grid.h"

#include <QVector2D>
#include <QPainter>

#include "gridtile.h"
#include "game/misc/tile.h"
#include "leveltile.h"

Grid::Grid(int width, int height, QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->width = width;
    this->height = height;

    this->setPos(TILE_SIZE, TILE_SIZE);
    for(int i = 1; i < height - 1; i++)
    {
        for(int j = 1; j < width - 1; j++)
            tiles.push_back(new GridTile(QPointF(j * TILE_SIZE + 50.0f, i * TILE_SIZE + 50.0f)));
    }
}

Grid::~Grid()
{
    for(int i = 0; i < tiles.length(); i++)
        delete tiles[i];
}

GridTile* Grid::getClosestTile(QPointF pos)
{
    if(tiles.size() == 0) return nullptr;
    GridTile* tile = tiles.first();
    QVector2D distance = QVector2D(tile->getPos()) - QVector2D(pos);
    for(int i = 0; i < tiles.length(); i++)
    {
        QVector2D d = QVector2D(tiles[i]->getPos()) - QVector2D(pos);
        if(distance.length() > d.length())
        {
            tile = tiles[i];
            distance = d;
        }
    }

    return tile;
}

QList<QString> Grid::getLevelData()
{
    QList<QString> data;
    QString line;
    for(int i = 0; i < width; i++)
        line += "W";
    data.push_back(line);
    line = "";

    for(int i = 0; i < (height - 2); i++)
    {
        line += "W";
        for(int j = 0; j < (width - 2); j++)
        {
            LevelTile* tile = tiles[(i * (width - 2)) + j]->getTile();
            if(!tile)
            {
                line += " ";
                continue;
            }

            switch(tile->getType())
            {
                case TileItem::TileType::BOX: line += "B"; break;
                case TileItem::TileType::COMPUTER: line += "C"; break;
                case TileItem::TileType::BOOKSHELF: line += "F"; break;
                case TileItem::TileType::PLAYER: line += "P"; break;
                case TileItem::TileType::SCIENTIST: line += "S"; break;
                case TileItem::TileType::ZOMBIE: line += "Z"; break;
                default: line += " "; break;
            }
        }
        line += "W";
        data.push_back(line);
        line = "";
    }

    for(int i = 0; i < width; i++)
        line += "W";
    data.push_back(line);

    return data;
}

GridTile* Grid::getPlayerTile()
{
    GridTile* tile = nullptr;
    for(int i = 0; i < tiles.length(); i++)
    {
        if(!tiles[i]->isOccupied()) continue;
        if(tiles[i]->getTile()->getType() == TileItem::TileType::PLAYER) tile = tiles[i];
    }

    return tile;
}

QRectF Grid::boundingRect() const
{
    return QRect(0.0f, 0.0f, (width - 2) * TILE_SIZE, (height - 1) * TILE_SIZE);
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(QBrush(QColor(0, 150, 0)), 3.0f));
    painter->drawRect(0.0f, 0.0f, (width - 2) * TILE_SIZE, (height - 2) * TILE_SIZE);

    for(int i = 1; i < height - 1; i++)
        painter->drawLine(i * TILE_SIZE, 0, i * TILE_SIZE, (height - 2) * TILE_SIZE);

    for(int i = 1; i < width - 1; i++)
        painter->drawLine(0, i * TILE_SIZE, (width - 2) * TILE_SIZE, i * TILE_SIZE);
}
