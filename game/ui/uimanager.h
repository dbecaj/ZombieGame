#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QGraphicsItemGroup>

#include "uicomponent.h"

class UIManager : public QObject, public QGraphicsItemGroup
{   
    QList<UIComponent*> components;
public:
    explicit UIManager(QObject* parent = nullptr);
    ~UIManager();
    void update();
    void addComponent(UIComponent* component);
};

#endif // UIMANAGER_H
