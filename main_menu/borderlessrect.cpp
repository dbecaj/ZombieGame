#include "borderlessrect.h"

#include <QPainter>
#include <QDebug>

BorderlessRect::BorderlessRect()
{
    this->color = QColor(255, 255, 255);
}

BorderlessRect::BorderlessRect(QRectF rect, QColor color) : QGraphicsRectItem(rect)
{
    this->color = color;
}

BorderlessRect::BorderlessRect(float x, float y, float width, float height, QColor color) : QGraphicsRectItem(x, y, width, height)
{
    this->color = color;
}

void BorderlessRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(color));
    painter->setPen(color);
    painter->drawRect(this->boundingRect());
}
