#ifndef MAINMENU_H
#define MAINMENU_H

#include <QGraphicsScene>

#include "inputpage.h"
#include "core/scene.h"

class InputManager;
class Button;
class MultiplayerPage;
class LevelSizePage;
class LevelSelectorPage;

class MainMenu : public Scene
{
    Q_OBJECT

    MultiplayerPage* multiplayerPage;
    LevelSizePage* levelSizePage;
    LevelSelectorPage* levelPage;


    InputManager* inputManager;
    Button* playButton;
    Button* quitButton;
    Button* multiplayerButton;
    Button* createButton;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
private slots:
    void clickPlayButton(); // Launches normal game
    void clickQuitButton(); // Closes the game
    void clickMultiplayerButton(); // Launches multiplayer page
    void clickCreateButton(); // Launches create level editor
public:
    explicit MainMenu(QGraphicsView* parent = 0);

    void init();
    void cleanUp();

    MultiplayerPage* getMultiplayerPage() { return multiplayerPage; }
    LevelSelectorPage* getLevelPage() { return levelPage; }
};

#endif // MAINMENU_H
