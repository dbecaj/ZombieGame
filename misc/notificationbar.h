#ifndef NOTIFICATIONBAR_H
#define NOTIFICATIONBAR_H

#include <QGraphicsTextItem>
#include <QObject>

#include "main_menu/borderlessrect.h"
#include "text.h"

class NotificationBar : public QObject, public BorderlessRect
{
    Q_OBJECT

    QGraphicsTextItem* displayText;
public:
    static const float HEIGHT;

    explicit NotificationBar(QObject* parent = nullptr);
    void setPosition(float x, float y);
    void setPosition(QPointF pos);
    void setText(Text text);
    void setRectColor(QColor rectColor);
};

#endif // NOTIFICATIONBAR_H
