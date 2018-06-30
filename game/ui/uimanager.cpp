#include "uimanager.h"

#include <QDebug>

#include "core/window.h"

extern Window* w;
UIManager::UIManager(QObject *parent) : QObject(parent)
{
    this->setPos(0.0f, 0.0f);
    this->setHandlesChildEvents(false);
}

UIManager::~UIManager()
{
    components.clear();
}

void UIManager::addComponent(UIComponent *component)
{
    this->addToGroup(component);
    components.push_back(component);
}

void UIManager::update()
{
    this->setPos(w->mapToScene(0, 0));
    for(int i = 0; i < components.length(); i++)
        components[i]->update();
}
