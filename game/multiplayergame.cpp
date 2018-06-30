#include "multiplayergame.h"

#include <QKeyEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>

#include "core/window.h"
#include "network/client.h"
#include "game/entities/mobs/player.h"
#include "ui/pausemenu.h"
#include "main_menu/mainmenu.h"
#include "main_menu/multiplayerpage.h"
#include "misc/gun.h"
#include "entities/bullet.h"
#include "entities/mobs/zombie.h"
#include "entities/mobs/scientist.h"
#include "network/packets/packetentitydata.h"
#include "network/packets/packetleveldata.h"
#include "network/packets/packetplayerpos.h"
#include "network/packets/packetzombiechangetarget.h"
#include "network/packets/packetscientistchangedir.h"
#include "network/packets/packetfirebullet.h"
#include "network/packets/packetzombiehit.h"
#include "network/packets/packetbullethit.h"
#include "network/packets/packetplayerchat.h"
#include "network/packets/packetinfected.h"
#include "network/packets/packetdisconnect.h"
#include "network/packets/packetjoin.h"
#include "network/packets/packetkick.h"
#include "misc/musicmanager.h"
#include "misc/notification.h"
#include "network/packets/packetentityupdate.h"

extern Window* w;
MultiplayerGame::MultiplayerGame(QGraphicsView *parent) : Game(parent, SceneType::MULTIPLAYER_GAME)
{
    serverIP = QHostAddress("0.0.0.0"); // uninitialized address
    insertingText = false;
}

MultiplayerGame::~MultiplayerGame()
{
}

void MultiplayerGame::prepare()
{
    qDebug() << "preparing";
    this->serverIP = w->getClient()->getTcpSocket()->peerAddress();

    this->connect(w->getClient(), SIGNAL(udpPacketReceived(QHostAddress,int,QByteArray)), this, SLOT(processUdpPacket(QHostAddress,int,QByteArray)));
    this->connect(w->getClient(), SIGNAL(tcpPacketReceived(QTcpSocket*,QByteArray)), this, SLOT(processTcpPacket(QTcpSocket*,QByteArray)));
    this->connect(w->getClient()->getTcpSocket(), SIGNAL(disconnected()), this, SLOT(quitGame()));
    qDebug() << "done!";
}

void MultiplayerGame::init()
{
    qDebug() << "initializing!";
    static bool first = true;

    if(!pauseMenu)
    {
        pauseMenu = new PauseMenu(this);
    }

    w->setBackgroundBrush(QBrush(QImage(":/images/textures/tile.png")));
    frameCounter = 0;

    // We don't connect any player mobHit(Mob*) signals or zombie mobHit(Mob*) signal because the server handles all that
    level.loadLevel(this);
    player = new Player(level.playerStartPos);
    this->connect(player, SIGNAL(shotBullet()), this, SLOT(playerShot()));
    this->addItem(player);
    player->enableNameTag(w->getUsername());

    if(first) this->connect(gameLoopTimer, SIGNAL(timeout()), SLOT(gameLoop()));
    gameLoopTimer->start();
    gameState = GameState::RUNNING;

    chatBox = new ChatBox(5.0f, w->getWindowHeight() - 580.0f, 300.0f, 400.0f);
    this->addItem(chatBox);
    chatBox->setZValue(0.01f);
    chatBox->getInsertText()->hide();
    chatBox->getInsertText()->setCursorDisplayLimit(280.0f);
    chatBox->getInsertText()->changeTextColor(QColor(255, 255, 255));

    w->setCurrentScene(this);

    reset = true;
    first = false;

    MusicManager::stop();
    qDebug() << "done!";
}

void MultiplayerGame::resetGame()
{
    qDebug() << "reset game!";
    for(PlayerMP* p : players)
    {
        if(!removePlayer(p)) qDebug() << "can't delete player!";
    }
    for(ScientistMP* s : scientists)
    {
        if(!removeScientist(s)) qDebug() << "can't delete scientist!";
    }
    for(ZombieMP* z : zombies)
    {
        if(!removeZombie(z)) qDebug() << "can't delete zombie!";
    }

    players.clear();
    scientists.clear();
    zombies.clear();

    if(!player)
    {
        player = new Player(level.playerStartPos);
        this->connect(player, SIGNAL(shotBullet()), this, SLOT(playerShot()));
        this->addItem(player);
        player->enableNameTag(w->getUsername());
    }
    else player->setPos(level.playerStartPos);

    chatBox->addMessage("SERVER","Game restarted!");
}

void MultiplayerGame::cleanUp()
{
    gameLoopTimer->stop();

    qDebug() << "clean loop";
    for(QGraphicsItem* item : items())
        delete item;
    qDebug() << "clean loop done!";

    pauseMenu = nullptr;
    player = nullptr;
    scientists.clear();
    zombies.clear();
    players.clear();
    tiles.clear();
    gameState = GameState::STOPPED;
    reset = false;
}

void MultiplayerGame::quitGame()
{
    qDebug() << "quit game!";
    cleanUp();
    qDebug() << "cleanUp()";
    PacketDisconnect packet(w->getUsername());
    w->getClient()->sendPacket(serverIP, SERVER_PORT, &packet, Protocol::TCP);

    this->disconnect(w->getClient()->getTcpSocket(), SIGNAL(disconnected()), this, SLOT(quitGame()));
    this->disconnect(w->getClient(), SIGNAL(udpPacketReceived(QHostAddress,int,QByteArray)), this, SLOT(processUdpPacket(QHostAddress,int,QByteArray)));
    this->disconnect(w->getClient(), SIGNAL(tcpPacketReceived(QTcpSocket*,QByteArray)), this, SLOT(processTcpPacket(QTcpSocket*,QByteArray)));

    w->getMainMenu()->getMultiplayerPage()->resetContent();
    w->setCurrentScene(w->getMainMenu()->getMultiplayerPage());
    qDebug() << "quitGame() done!";
}

void MultiplayerGame::gameLoop()
{
    if(gameState == GameState::RUNNING)
    {
        if(player) handleInput();
        else spectator.update();
    }

    this->handleCollision();
    updateAll();
    chatBox->setPos(w->mapToScene(5.0f, 140.0f));
    this->update(this->sceneRect());
    this->sortForDrawing();
    frameCounter++;
}

void MultiplayerGame::updateAll()
{
    if(player) player->update(); // gun->update();
    for(int i = 0; i < players.length(); i++)
        players[i]->update();

    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->update();

    for(int i = 0; i < scientists.length(); i++)
        scientists[i]->update();
}

void MultiplayerGame::playerShot()
{
    Bullet* bullet = player->getCurrentGun()->getBullets().last();
    PacketFireBullet fireBulletPacket(w->getUsername(), bullet->pos(), bullet->getDirection());
    w->getClient()->sendPacket(serverIP, SERVER_PORT, &fireBulletPacket, Protocol::UDP);
}

void MultiplayerGame::handleInput()
{
    PacketPlayerPos playerPosPacket(w->getUsername(), player->pos(), player->getDegrees());
    w->getClient()->sendPacket(serverIP, SERVER_PORT, &playerPosPacket, Protocol::UDP);

    player->handleInput();
}

void MultiplayerGame::handleCollision()
{
    if(player) player->entityCollision();
    for(int i = 0; i < players.length(); i++)
        players[i]->entityCollision();
    for(int i = 0; i < scientists.length(); i++)
        scientists[i]->mobCollision();
    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->mobCollision();

    if(player) player->tileCollision();
    // We don't do playermp tile collision because we get the players position every 1/60 sec
    for(int i = 0; i < scientists.length(); i++)
        scientists[i]->tileCollision();
    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->tileCollision();
}

void MultiplayerGame::processUdpPacket(QHostAddress ip, int port, QByteArray data)
{
    QString ipAddress = ip.toString();
    if(ipAddress != "" && ipAddress.at(0) == ':') // if we get IPv6 (::ffff:) we just take the IPv4 part at the end
        ipAddress = ipAddress.mid(7);

    if(serverIP == QHostAddress("0.0.0.0")) return;
    if(QHostAddress(ipAddress) != serverIP) return; // we only accept our server

    Packet* packet = nullptr;
    PacketType type = Packet::determinePacketType(data);
    if(type == PacketType::PLAYER_POS) packet = new PacketPlayerPos(data);
    else if(type == PacketType::FIRE_BULLET) packet = new PacketFireBullet(data);
    else if(type == PacketType::SCIENTIST_CHANGE_DIR) packet = new PacketScientistChangeDir(data);
    else if(type == PacketType::ENTITY_UPDATE) packet = new PacketEntityUpdate(data);

    if(packet)
    {
        if(!packet->execute()) qDebug() << packet->getErrorMsg();
        delete packet;
    }
}

void MultiplayerGame::processTcpPacket(QTcpSocket *socket, QByteArray data)
{
    if(socket->peerAddress().toIPv4Address() != serverIP.toIPv4Address()) return;

    Packet* packet = nullptr;
    PacketType type = Packet::determinePacketType(data);
    if(type == PacketType::LEVEL_DATA)
    {
        packet = new PacketLevelData(data);
        PacketEntityData entitiesDataRequest = PacketEntityData();
        w->getClient()->sendPacket(QHostAddress(serverIP), SERVER_PORT, &entitiesDataRequest, Protocol::TCP);
    }
    else if(type == PacketType::ENTITY_DATA)
    {
        if(!reset) init();
        else resetGame();

        packet = new PacketEntityData(data);
    }
    else if(type == PacketType::ZOMBIE_HIT) packet = new PacketZombieHit(data);
    else if(type == PacketType::BULLET_HIT) packet = new PacketBulletHit(data);
    else if(type == PacketType::JOIN) packet = new PacketJoin(data);
    else if(type == PacketType::PLAYER_CHAT) packet = new PacketPlayerChat(data);
    else if(type == PacketType::KICK) packet = new PacketKick(data);
    else if(type == PacketType::INFECTED) packet = new PacketInfected(data);
    else if(type == PacketType::DISCONNECT) packet = new PacketDisconnect(data);
    else if(type == PacketType::ZOMBIE_CHANGE_TARGET) packet = new PacketZombieChangeTarget(data);
    else if(type == PacketType::SERVER_SHUTDOWN) quitGame();

    if(packet)
    {
        if(!packet->execute()) qDebug() << packet->getErrorMsg();
        delete packet;
    }
}

PlayerMP* MultiplayerGame::getPlayer(QString username)
{
    PlayerMP* player = nullptr;
    for(int i = 0; i < players.length(); i++)
    {
        if(players[i]->getUsername() == username)
        {
            player = players[i];
            return player;
        }
    }

    if(username == w->getUsername()) return (PlayerMP*)this->player;

    return player;
}

ZombieMP* MultiplayerGame::getZombie(long id)
{
    for(int i = 0; i < zombies.length(); i++)
        if(zombies[i]->getId() == id) return zombies[i];

    return nullptr;
}

ScientistMP* MultiplayerGame::getScientist(long id)
{
    for(int i = 0; i < scientists.length(); i++)
        if(scientists[i]->getId() == id) return scientists[i];

    return nullptr;
}

QList<Mob*> MultiplayerGame::getMobs()
{
    QList<Mob*> mobs;
    if(player) mobs.push_back(player);
    for(int i = 0; i < players.length(); i++) mobs.push_back(players[i]);
    for(int i = 0; i < zombies.length(); i++) mobs.push_back(zombies[i]);
    for(int i = 0; i < scientists.length(); i++) mobs.push_back(scientists[i]);

    return mobs;
}

QList<Scientist*> MultiplayerGame::getScientists()
{
    QList<Scientist*> list;
    for(int i = 0; i < scientists.length(); i++)
        list.push_back(scientists[i]);

    return list;
}

void MultiplayerGame::addZombie(ZombieMP *zombie)
{
    zombies.push_back(zombie);
    this->addItem(zombie);
}

void MultiplayerGame::addPlayer(PlayerMP *player)
{
    players.push_back(player);
    this->addItem(player);
}

void MultiplayerGame::addScientist(ScientistMP *scientist)
{
    scientists.push_back(scientist);
    this->addItem(scientist);
}

bool MultiplayerGame::removeZombie(ZombieMP *zombie)
{
    if(!zombies.removeOne(zombie)) return false;

    delete zombie;

    return true;
}

bool MultiplayerGame::removePlayer(PlayerMP *player)
{
    if(player == this->player)
    {
        qDebug() << "player is nullptr!";
        this->player = nullptr;
    }
    else if(!players.removeOne(player)) return false;

    player->getCurrentGun()->removeAllBullets();
    delete player;

    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->setTarget(zombies.at(i)->scanForTarget());

    return true;
}

bool MultiplayerGame::removePlayer(QString username)
{
    qDebug() << "removing player " << username;
    PlayerMP* player = getPlayer(username);
    if(!player && username != w->getUsername()) return false;

    qDebug() << "done!";
    return removePlayer(player);
}

bool MultiplayerGame::removeScientist(ScientistMP *scientist)
{
    if(!scientists.removeOne(scientist)) return false;

    delete scientist;

    for(int i = 0; i < zombies.length(); i++)
        zombies[i]->setTarget(zombies.at(i)->scanForTarget());

    return true;
}

void MultiplayerGame::keyPressEvent(QKeyEvent *event)
{
    if(w->getCurrentScene()->getSceneType() != SceneType::MULTIPLAYER_GAME) return;
    if(event->key() == Qt::Key_Escape)
    {
        if(gameState != GameState::PAUSED)
        {
            gameState = GameState::PAUSED;
            QSize windowSize(500, 600);
            QPointF scenePos = w->mapToScene(w->getWindowWidth()/2 - windowSize.width()/2, 50);
            pauseMenu->setPos(scenePos);
            this->addItem(pauseMenu);
            pauseMenu->setZValue(0.011f);
        }
        else
        {
            gameState = GameState::RUNNING;
            this->removeItem(pauseMenu);
        }
    }
    else if(event->key() == Qt::Key_Return)
    {
        if(insertingText)
        {
            PacketPlayerChat playerChatPacket(w->getUsername(), chatBox->getInsertText()->getText());
            w->getClient()->sendPacket(serverIP, CLIENT_PORT, &playerChatPacket, Protocol::TCP);
            chatBox->addMessage();
            gameState = GameState::RUNNING;
            chatBox->getInsertText()->hide();
            chatBox->getInsertText()->clearText();
            insertingText = false;
        }
        else
        {
            gameState = GameState::PAUSED;
            chatBox->getInsertText()->show();
            insertingText = true;
        }
    }
    else
    {
        if(insertingText) chatBox->getInsertText()->keyPressEvent(event);
    }
    QGraphicsScene::keyPressEvent(event);
}

void MultiplayerGame::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->accept();
    if(!insertingText)
    {
        QGraphicsScene::wheelEvent(event);
        return;
    }

    if(event->delta() > 0) chatBox->scrollUp();
    else chatBox->scrollDown();
}

void MultiplayerGame::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(!player) // player is infected!
        {
           spectator.next();
        }
    }

    QGraphicsScene::mousePressEvent(event);
}
