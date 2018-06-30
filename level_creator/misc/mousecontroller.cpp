#include "mousecontroller.h"

#include <QScrollBar>
#include <qmath.h>

#include "level_creator/levelcreator.h"
#include "core/window.h"
#include "core/inputmanager.h"
#include "game/misc/tile.h"
#include "level_creator/grid/grid.h"
#include "level_creator/grid/gridtile.h"

extern Window* w;
MouseController::MouseController()
{
}

void MouseController::update()
{
    float speed = 10.0f;
    QPointF centerPoint = w->mapToScene(w->getWindowWidth()/2 - 1, w->getWindowHeight()/2 - 1);
    QPointF cursorPos = w->mapToScene(w->getInputManager()->mousePosition.toPoint());
    // we subtract 200.0f to create a rectangle around the view with a 100.0f border all around
    QPointF topPoint = w->mapToScene(25.0f, 25.0f);
    QPointF bottomPoint(topPoint.x() + w->getWindowWidth() - 50.0f, topPoint.y() + w->getWindowHeight() - 50.0f);
    QRectF viewRect(topPoint, bottomPoint);
    if(!viewRect.contains(cursorPos))
    {
        QPointF fastPointTop(topPoint.x() - 12.0f, topPoint.y() - 12.0f);
        QPointF fastPointBottom(bottomPoint.x() + 12.0f, bottomPoint.y() + 12.0f);
        QRectF fastRect(fastPointTop, fastPointBottom);
        if(!fastRect.contains(cursorPos)) speed = 20.0f;

        if(cursorPos.x() < viewRect.x()) centerPoint.setX(centerPoint.x() - speed);
        else if(cursorPos.x() > (viewRect.x() + viewRect.width())) centerPoint.setX(centerPoint.x() + speed);

        if(cursorPos.y() < viewRect.y()) centerPoint.setY(centerPoint.y() - speed);
        else if(cursorPos.y() > (viewRect.y() + viewRect.height())) centerPoint.setY(centerPoint.y() + speed);
    }
    w->centerOn(centerPoint);
}
