#ifndef LISTITEM_H
#define LISTITEM_H

#include <QGraphicsItemGroup>

class ListItem : public QGraphicsItemGroup // This could easily be a QGraphicsItem and do just fine if not better
{
protected:
    QGraphicsPixmapItem* container;
public:
    explicit ListItem(float x, float y, float width, float height, QGraphicsItem* parent = nullptr);
};

#endif // LISTITEM_H
