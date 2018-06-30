#include "button.h"

#include <QDebug>
#include <QFontMetrics>
#include <QPainter>

#include "mainmenu.h"

Button::Button(float x, float y, float width, float height, QPixmap pixmapIdle, QPixmap pixmapActive, Text text, QGraphicsItem *parent) : QGraphicsItemGroup(parent)
{
    // Scale to size just in case
    this->setPos(x, y);

    QPixmap tmp = pixmapIdle.scaled(width, height); // I have to create the tmp because if I don't the pixmap doesn't update
    this->pixmapIdle = pixmapIdle.scaled(width, height);
    this->pixmapActive = pixmapActive.scaled(width, height);
    this->text = text;
    this->textColorIdle = text.getColor();
    this->textColorActive = text.getColor();
    enabled = true;

    background = new QGraphicsPixmapItem(this);
    background->setPixmap(tmp);
    this->addToGroup(background);
    background->setPos(0.0f, 0.0f);

    // We must set the AcceptHoverEvents to true for the hoverEnterEvent and hoverLeaveEvent to fire
    this->setAcceptHoverEvents(true);

    displayText = new QGraphicsTextItem(this);
    displayText->setPlainText(text.getText());
    displayText->setDefaultTextColor(text.getColor());
    displayText->setFont(text.getFont());
    this->addToGroup(displayText);
    QFontMetrics fM(text.getFont());
    displayText->setPos(width/2.0f - fM.width(text.getText())/2.0f, height/2.0f - fM.height()/2.0f - 4.4f);
}

void Button::setButtonEnabled(bool enabled)
{
    this->enabled = enabled;
    if(enabled) this->setOpacity(1.0f);
    else this->setOpacity(0.5f);
    this->update();
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(enabled) emit clickEvent();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(!enabled) return;

    background->setPixmap(pixmapActive);
    displayText->setDefaultTextColor(textColorActive);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(!enabled) return;

    background->setPixmap(pixmapIdle);
    displayText->setDefaultTextColor(textColorIdle);
}
