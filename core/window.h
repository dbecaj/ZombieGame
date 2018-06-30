#ifndef WINDOW_H
#define WINDOW_H

#include <QGraphicsView>
#include <QString>

#include "network/client.h"
#include "config.h"

class InputManager;
class QCloseEvent;
class QKeyEvent;
class QMouseEvent;
class MainMenu;
class Game;
class MultiplayerGame;
class LevelCreator;
class Scene;
class InputPage;

class Window : public QGraphicsView
{
    Q_OBJECT

    int width, height;
    float widthScale;
    float heightScale;
    QString title;
    InputManager* inputManager;
    Scene* currentScene;
    Game* game;
    MultiplayerGame* multiplayerGame;
    LevelCreator* levelCreator;
    MainMenu* mainMenu;
    InputPage* inputPage;
    Client client;
    Config* config;
    QString username;

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

public:
    explicit Window(int width, int height, QString title, QGraphicsView* parent = 0);
    ~Window();

    void init();
    void setCurrentScene(Scene* scene);

    float getWidthScale() { return widthScale; }
    float getHeightScale() { return heightScale; }
    InputManager* getInputManager() { return inputManager; }
    int getWindowWidth() { return width; }
    int getWindowHeight() { return height; }
    QString getWindowTitle() { return title; }
    Game* getGame() { return game; }
    MainMenu* getMainMenu() { return mainMenu; }
    Config* getConfig() { return config; }
    Client* getClient() { return &client; }
    QString getUsername() { return username; }
    MultiplayerGame* getMultiplayerGame() { return multiplayerGame; }
    LevelCreator* getLevelCreator() { return levelCreator; }
    Scene* getCurrentScene() { return currentScene; }
};

#endif // WINDOW_H
