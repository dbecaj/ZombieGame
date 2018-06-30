#ifndef SCORE_H
#define SCORE_H

#include <QObject>

#include "uicomponent.h"

class QGraphicsTextItem;
class UIManager;

class Score : public QObject, public UIComponent
{
    Q_OBJECT

    QGraphicsTextItem* scientistText; // Shows the number of scientists still in the game
    QGraphicsTextItem* zombieText; // Shows the number of zombies  in the game
public:
    Score();

    void update(); // Updates all texts
};

#endif // SCORE_H
