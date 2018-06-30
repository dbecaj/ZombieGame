#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsPixmapItem>

#define TILE_ITEM_SIZE 100.0f

class TileItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
protected:
    QPixmap pixmap;
signals:
    void clickEvent(TileItem* item);
public:
    enum class TileType
    {
        BOX,
        COMPUTER,
        BOOKSHELF,
        PLAYER,
        SCIENTIST,
        ZOMBIE
    };

    explicit TileItem(QPixmap pixmap, QPointF pos, TileType type, QObject* parent = nullptr);

    QPixmap getPixmap() { return pixmap; }
private:
    TileType type;
public:
    TileType getType() { return type; }

    QRectF boundingRect() const;
};

#endif // TILEITEM_H
