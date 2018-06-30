#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <QGraphicsItemGroup>
#include <QObject>

#include "game/game.h"

class Button;

class PauseMenu : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    QGraphicsPixmapItem* background;
    Button* mainMenuButton;
    float lastY;
    float buttonX;
    QSize buttonSize;
private slots:
    void clickMainMenuButton(); // Is called when mainMenuButton is clicked
public:
    explicit PauseMenu(QObject *parent);

    void addButton(Button* button, QObject *maker, const char *slot);
};

#endif // PAUSEMENU_H
