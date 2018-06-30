#ifndef MULTIPLAYERGAME_H
#define MULTIPLAYERGAME_H

#include <QHostAddress>
#include <QTcpSocket>

#include "game.h"
#include "network/packets/packet.h"
#include "network/entities/playermp.h"
#include "network/entities/scientistmp.h"
#include "network/entities/zombiemp.h"
#include "ui/chatbox.h"
#include "misc/spectator.h"

class MultiplayerGame : public Game
{
    Q_OBJECT

    ChatBox* chatBox;
    QHostAddress serverIP;
    QList<PlayerMP*> players;
    QList<ZombieMP*> zombies;
    QList<ScientistMP*> scientists;
    bool insertingText;
    Spectator spectator;

    void handleInput(); // Handles players input and sends it to the server
    void updateAll(); // General update function that updates all mobs
    void resetGame();

    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private slots:
    void gameLoop(); // This is main game loop and its run at 60FPS
    void processUdpPacket(QHostAddress ip, int port, QByteArray data); // Does appropriate action with the packet
    void processTcpPacket(QTcpSocket* socket, QByteArray data); // Does appropriate action with the packet
    void playerShot();
public slots:
    void quitGame(); // Does cleanUp(), disconnects the necessary signals and changes the scene to mainMenu
public:
    explicit MultiplayerGame(QGraphicsView *parent = nullptr);
    virtual ~MultiplayerGame();

    void prepare(); // Gets the ip from the client and connects the necessary signals
    void init();
    void cleanUp(); // Stops the gameLoop and cleans the scene and other objects
    void handleCollision();

    QHostAddress getServerIP() { return serverIP; }
    PlayerMP* getPlayer(QString username);
    ZombieMP* getZombie(long id);
    ScientistMP* getScientist(long id);
    QList<Scientist*> getScientists();
    QList<PlayerMP*> getPlayers() { return players; }
    QList<ZombieMP*> getZombies() { return zombies; }
    QList<Mob*> getMobs();
    ChatBox* getChatBox() { return chatBox; }
    Level* getLevel() { return &level; }

    void addZombie(ZombieMP* zombie);
    void addPlayer(PlayerMP* player);
    void addScientist(ScientistMP* scientist);
    bool removeZombie(ZombieMP* zombie);
    bool removePlayer(PlayerMP* player);
    bool removePlayer(QString username);
    bool removeScientist(ScientistMP* scientist);
};

#endif // MULTIPLAYERGAME_H
