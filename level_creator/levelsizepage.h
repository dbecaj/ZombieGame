#ifndef LEVELSIZEPAGE_H
#define LEVELSIZEPAGE_H

#include "core/scene.h"

class InsertText;
class Button;

class LevelSizePage : public Scene
{
    Q_OBJECT

    void keyPressEvent(QKeyEvent* event);

    InsertText* insertName;
    InsertText* insertWidth;
    InsertText* insertHeight;
    Button* backButton;
    QGraphicsPixmapItem* background;
private slots:
    void backButtonClicked();
public:
    explicit LevelSizePage(QObject* parent = nullptr);

    void init();
    void cleanUp();
};

#endif // LEVELSIZEPAGE_H
