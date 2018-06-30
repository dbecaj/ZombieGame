#include "levellistitem.h"

#include <QFontMetrics>
#include <QDebug>

#include "core/window.h"
#include "mainmenu.h"
#include "levelselectorpage.h"

extern Window* w;
LevelListItem::LevelListItem(float x, float y, QString levelName, QDate createdDate, QGraphicsItem *parent) : ListItem(x, y, LEVELLISTITEM_WIDTH,
                                                                                               LEVELLISTITEM_HEIGHT, parent)
{
    this->levelName = levelName;
    this->createdDate = createdDate;
    highlighted = false;

    QString name = levelName.split(".").first();
    levelNameDisplay = new QGraphicsTextItem(name);
    levelNameDisplay->setFont(QFont("VCR OSD Mono", 20));
    QFontMetrics fM(levelNameDisplay->font());
    this->addToGroup(levelNameDisplay);
    levelNameDisplay->setPos(LEVELLISTITEM_WIDTH/2.0f - fM.width(name)/2.0f, LEVELLISTITEM_HEIGHT/2.0f - fM.height() + 8.0f);

    QString date = QString::number(createdDate.day()) + "/" + QString::number(createdDate.month()) + "/" +
                   QString::number(createdDate.year());
    dateDisplay = new QGraphicsTextItem(date);
    dateDisplay->setFont(QFont("VCR OSD Mono", 14));
    fM = QFontMetrics(dateDisplay->font());
    this->addToGroup(dateDisplay);
    dateDisplay->setPos(LEVELLISTITEM_WIDTH - fM.width(date) - 15.0f, LEVELLISTITEM_HEIGHT - fM.height() - 10.0f);
}

QRectF LevelListItem::boundingRect() const
{
    return QRect(0.0f, 0.0f, LEVELLISTITEM_WIDTH, LEVELLISTITEM_HEIGHT);
}

void LevelListItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(highlighted)
    {
        QPainterPath path;
        path.addRect(container->x(), container->y(), container->boundingRect().width(), container->boundingRect().height());
        QPen pen(QColor(125, 125, 0), 10);
        painter->setPen(pen);
        painter->drawPath(path);
    }

    QGraphicsItemGroup::paint(painter, option, widget);
}

void LevelListItem::setHighlighted(bool highlighted)
{
    this->highlighted = highlighted;
    this->update();
}

void LevelListItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setHighlighted(true);
    w->getMainMenu()->getLevelPage()->changeButtonsStatus(true);
}

void LevelListItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(levelName);
}
