#include "notificationbar.h"

#include <QFontMetrics>

#include "core/window.h"

extern Window* w;
const float NotificationBar::HEIGHT = 60.0f;

NotificationBar::NotificationBar(QObject *parent) : QObject(parent)
{
    displayText = nullptr;
}

void NotificationBar::setPosition(float x, float y)
{
    NotificationBar::setPosition(QPointF(x, y));
}

void NotificationBar::setPosition(QPointF pos)
{
    this->setRect(0.0f, 0.0f, w->getWindowWidth(), NotificationBar::HEIGHT);
    this->setPos(pos);
}

void NotificationBar::setText(Text text)
{
    if(!displayText) displayText = new QGraphicsTextItem(this);
    displayText->setDefaultTextColor(text.getColor());
    displayText->setFont(text.getFont());
    displayText->setPlainText(text.getText());
    QFontMetrics fM(text.getFont());
    displayText->setPos(this->rect().width()/2.0f - fM.width(text.getText())/2.0f,
                        this->rect().height()/2.0f - fM.height()/2.0f - 7.0f);
}

void NotificationBar::setRectColor(QColor rectColor)
{
    this->setColor(rectColor);
}
