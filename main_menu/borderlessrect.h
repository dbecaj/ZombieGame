#ifndef BORDERLESSRECT_H
#define BORDERLESSRECT_H

#include <QGraphicsRectItem>

class BorderlessRect : public QGraphicsRectItem
{
    QColor color;
public:
    BorderlessRect();
    BorderlessRect(QRectF rect, QColor color);
    BorderlessRect(float x, float y, float width, float height, QColor color);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setColor(QColor color) { this->color = color; }
};

#endif // BORDERLESSRECT_H
