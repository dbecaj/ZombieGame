#include "listitem.h"

#include <QBrush>

ListItem::ListItem(float x, float y, float width, float height, QGraphicsItem *parent) : QGraphicsItemGroup(parent)
{
    // Set the HandlesChildEvents to false so all ListItems can process events like keyEvent(), mouseEvent(),...
    this->setHandlesChildEvents(false);
    this->setPos(x, y);
    container = new QGraphicsPixmapItem(this);
    QPixmap listFrame = QPixmap(":/images/textures/list_frame.png").scaled(width, height);
    container->setPixmap(listFrame);
    this->addToGroup(container);
}
