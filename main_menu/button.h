#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>

#include "misc/text.h"

class Button : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    QPixmap pixmapIdle; // Image for button when no one is hovering over it
    QPixmap pixmapActive; // Image for button when someone is hovering over it
    QGraphicsPixmapItem* background;
    QColor textColorIdle;
    QColor textColorActive;
    bool enabled;

    Text text;
    QGraphicsTextItem* displayText;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    void clickEvent(); // Fired when we click the button
public:
    explicit Button(float x, float y, float width, float height, QPixmap pixmapIdle, QPixmap pixmapActive, Text text = Text(), QGraphicsItem *parent = 0);
    void setActiveTextColor(QColor color) { this->textColorActive = color; }

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    QPixmap getIdlePixmap() { return pixmapIdle; }
    void setButtonEnabled(bool enabled);
};

#endif // BUTTON_H
