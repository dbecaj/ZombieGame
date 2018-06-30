#include "window.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QDebug>

#include "inputmanager.h"
#include "game/game.h"
#include "game/multiplayergame.h"
#include "main_menu/mainmenu.h"
#include "main_menu/multiplayerpage.h"
#include "level_creator/levelcreator.h"
#include "misc/language.h"
#include "misc/notification.h"
#include "misc/userdata.h"
#include "misc/musicmanager.h"

extern Window* w;
Window::Window(int width, int height, QString title, QGraphicsView* parent) : QGraphicsView(parent)
{   
    this->width = width;
    this->height = height;
    widthScale = width / 1920.0f;
    heightScale = height / 1080.0f;
    this->title = title;

    // Initialize the window properties
    this->setFixedSize(width, height);
    this->setWindowTitle(title);
    // Disable both scroll bars so that when we move entities the scroll bars don't show
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Enable the mouse tracking by the window so we can get mouse coords
    this->setMouseTracking(true);
    this->setBackgroundBrush(QBrush(QPixmap(":/images/textures/background.png")));

    inputManager = new InputManager();

    currentScene = nullptr;
    inputPage = nullptr;
    game = nullptr;
    multiplayerGame = nullptr;
    levelCreator = nullptr;
    mainMenu = nullptr;
    config = nullptr;
}

void Window::init()
{
    client.bindUdp(CLIENT_PORT);

    config = new Config();

    bool found = false;
    config->read("LangPack", &found);
    if(!found) config->write("LangPack", "lang_eng.xml");
    Language::init(QCoreApplication::applicationDirPath() + "/lang/" + config->read("LangPack"));

    mainMenu = new MainMenu(this);
    mainMenu->init();
    inputPage = new InputPage(this);

    UserData userData("user.dat");
    for(int i = 0; i < userData.getServerData().length(); i++)
        mainMenu->getMultiplayerPage()->addServer(userData.getServerData()[i]);

    // Check if the config.txt contains PlayerName
    found = false;
    config->read("PlayerName", &found);
    if(found) setCurrentScene(mainMenu);
    else
    {
        inputPage->init();
        setCurrentScene(inputPage);
    }
    username = config->read("PlayerName");

    game = new Game(this);
    multiplayerGame = new MultiplayerGame(this);
    levelCreator = new LevelCreator(this);
}

Window::~Window()
{
    // We only need to delete objects that aren't covered by QT automatic clean up (have no QObject* parent)
    delete inputManager;
}

void Window::setCurrentScene(Scene *scene)
{
    this->setScene(scene);

    if(scene->getSceneType() != SceneType::GAME && scene->getSceneType() != SceneType::MULTIPLAYER_GAME && scene->getSceneType() != SceneType::LEVEL_CREATOR)
        w->setBackgroundBrush(QBrush(QPixmap(":images/textures/background.png")));
    if(currentScene) currentScene->cleanUp();
    currentScene = scene;
}

void Window::closeEvent(QCloseEvent *event)
{   
    if(currentScene->getSceneType() == SceneType::GAME ||
       currentScene->getSceneType() == SceneType::MULTIPLAYER_GAME)
    {
        Game* game = (Game*)currentScene;
        game->quitGame();
    }
    else currentScene->cleanUp();

    MusicManager::stop();

    UserData::save("user.dat");
    this->deleteLater();
}

void Window::keyPressEvent(QKeyEvent *event)
{
    inputManager->pressKey(event->key());
    // For each event we propagate it down to other child classes
    QGraphicsView::keyPressEvent(event);
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
    inputManager->releaseKey(event->key());
    QGraphicsView::keyReleaseEvent(event);
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    inputManager->mousePosition = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void Window::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) inputManager->mouseButtonPressed = true;
    QGraphicsView::mousePressEvent(event);
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) inputManager->mouseButtonPressed = false;
    QGraphicsView::mouseReleaseEvent(event);
}
