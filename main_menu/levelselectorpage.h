#ifndef LEVELSELECTORPAGE_H
#define LEVELSELECTORPAGE_H

#include "core/scene.h"
#include "button.h"
#include "levellistitem.h"

class LevelSelectorPage : public Scene
{
    Q_OBJECT

    QList<LevelListItem*> levelList;
    Button* backButton;
    Button* loadButton;
    Button* deleteButton;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
private slots:
    void levelClick(QString levelName);
    void backButtonClick();
    void loadButtonClick();
    void deleteButtonClick();
public:
    LevelSelectorPage(QObject* parent = nullptr);

    void init();
    void cleanUp();

    void changeButtonsStatus(bool status);
    void removeLevel(int index);
};

#endif // LEVELSELECTORPAGE_H
