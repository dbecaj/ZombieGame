#ifndef MULTIPLAYERPAGE_H
#define MULTIPLAYERPAGE_H

#include <QList>

#include "core/scene.h"

class ServerListItem;
class Button;
class AddServerPopup;

class MultiplayerPage : public Scene
{
    Q_OBJECT

    QList<ServerListItem*> serverList; // All our servers
    Button* backButton;
    Button* addServerButton;
    Button* refreshButton;
    Button* connectButton;
    Button* deleteButton;
    AddServerPopup* addServerPopup;
    bool serverPopupOpen; // We use this to know when addServerPopup is open

    void wheelEvent(QGraphicsSceneWheelEvent *event); // TODO: implement this for scrolling though serverItems
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private slots:
    void clickBackButton(); // Changes the scene to mainMenu
    void clickAddServerButton(); // Opens addServerPopup;
    void clickRefreshButton(); // Resets all serverList items
    void clickConnectButton();
    void clickDeleteButton();
public:
    explicit MultiplayerPage(QObject* parent = nullptr);
    ~MultiplayerPage();

    void init();
    void cleanUp();

    void resetContent(); // Resets all serverList items
    void stopTimers(); // Stops all serverList items timers
    void addServer(QString ip);
    void removeServer(int index);

    bool isServerPopupOpen() { return serverPopupOpen; }
    void setServerPopupOpen(bool serverPopupOpen) { this->serverPopupOpen = serverPopupOpen; }
    QList<ServerListItem*>* getServerList() { return &serverList; }
    Button* getConnectButton() { return connectButton; }
    Button* getDeleteButton() { return deleteButton; }
};

#endif // MULTIPLAYERPAGE_H
