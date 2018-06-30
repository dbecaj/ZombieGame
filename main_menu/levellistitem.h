#ifndef LEVELLISTITEM_H
#define LEVELLISTITEM_H

#include <QObject>
#include <QDate>

#include "listitem.h"

#define LEVELLISTITEM_WIDTH 500.0f
#define LEVELLISTITEM_HEIGHT 70.0f

class LevelListItem : public QObject, public ListItem
{
    Q_OBJECT

    QString levelName;
    QDate createdDate;
    QGraphicsTextItem* levelNameDisplay;
    QGraphicsTextItem* dateDisplay;
    bool highlighted;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
signals:
    void clicked(QString levelName);
public:
    LevelListItem(float x, float y, QString levelName, QDate createdDate, QGraphicsItem* parent = nullptr);

    QString getLevelName() { return levelName; }
    void setHighlighted(bool highlighted);
    bool isHighlighted() { return highlighted; }

    QRectF boundingRect() const;
};

#endif // LEVELLISTITEM_H
