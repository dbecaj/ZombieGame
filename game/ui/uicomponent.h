#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include <QGraphicsItem>

class UIComponent : public QGraphicsItemGroup
{
public:
    UIComponent(QGraphicsItem* parent = nullptr);
    virtual void update() = 0;
};

#endif // UICOMPONENT_H
