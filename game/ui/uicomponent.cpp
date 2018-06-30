#include "uicomponent.h"

#include <QDebug>

UIComponent::UIComponent(QGraphicsItem *parent) : QGraphicsItemGroup(parent)
{
    this->setHandlesChildEvents(false);
}
