#include "serverlistitem.h"

#include <QDebug>
#include <QGraphicsTextItem>
#include <QFont>
#include <QTimer>
#include <QColor>

#include "core/window.h"
#include "network/packets/packet.h"
#include "mainmenu.h"
#include "multiplayerpage.h"
#include "game/misc/level.h"
#include "game/multiplayergame.h"
#include "network/packets/packetinfo.h"
#include "network/packets/packetping.h"
#include "network/packets/packetjoin.h"
#include "network/packets/packetleveldata.h"
#include "misc/language.h"
#include "button.h"
#include "misc/notification.h"
#include "network/packets/packeterror.h"

extern Window* w;
ServerListItem::ServerListItem(float x, float y, QString serverIP, long id, QGraphicsItem *parent) : ListItem(x, y, SERVERLISTITEM_WIDTH, SERVERLISTITEM_HEIGHT, parent)
{
    this->id = id;
    ping = -1; // -1 ping is a sign the we can't connect to server
    step = 0;
    isEstablished = false;
    this->serverIP = serverIP;
    highlighted = false;

    this->connect(w->getClient(), SIGNAL(udpPacketReceived(QHostAddress, int, QByteArray)), this, SLOT(processUdpPacket(QHostAddress, int, QByteArray)));
    this->connect(w->getClient(), SIGNAL(tcpPacketReceived(QTcpSocket*,QByteArray)), this, SLOT(processTcpPacket(QTcpSocket*, QByteArray)));

    pingText = new QGraphicsTextItem(this);
    pingText->setDefaultTextColor(QColor(155, 0, 0));
    pingText->setFont(QFont("VCR OSD Mono", 15));
    pingText->setPlainText(QString::number(ping) + QString("ms"));
    pingText->setPos(SERVERLISTITEM_WIDTH - 75.0f, 5.0f);

    serverNameText = new QGraphicsTextItem(this);
    serverNameText->setDefaultTextColor(Qt::black);
    serverNameText->setFont(QFont("VCR OSD Mono", 30));
    serverNameText->setPlainText("");
    serverNameText->setPos(25.0f, 25.0f);

    serverIPText = new QGraphicsTextItem(this);
    serverIPText->setDefaultTextColor(Qt::black);
    serverIPText->setFont(QFont("VCR OSD Mono", 15));
    serverIPText->setPlainText(serverIP);
    serverIPText->setPos(25.0f, 65.0f);

    serverCapacity = new QGraphicsTextItem(this);
    serverCapacity->setDefaultTextColor(Qt::black);
    serverCapacity->setFont(QFont("VCR OSD Mono", 20));
    serverCapacity->setPlainText("0/0");
    serverCapacity->setPos(SERVERLISTITEM_WIDTH - 105.0f, SERVERLISTITEM_HEIGHT - 55.0f);

    // Try to connect to the server
    connectingTimer = new QTimer(this);
    this->connect(connectingTimer, SIGNAL(timeout()), this, SLOT(connecting()));
    connectingTimer->start(200);

    myTcpSocket = new QTcpSocket(this);

    refreshTimer = new QTimer(this);
    refreshTimer->setInterval(200);
}

ServerListItem::~ServerListItem()
{
    refreshTimer->stop();
    connectingTimer->stop();
}

void ServerListItem::pingServer()
{
    // Include the id of this server in the packet so when we get it back we know who is it for
    PacketPing pingPacket(id);
    w->getClient()->sendPacket(QHostAddress(serverIP), SERVER_PORT, &pingPacket, Protocol::UDP);
    pingTimer.restart();

    if(ping > 999) // This much ping is usually server down
    {
        pingText->setDefaultTextColor(QColor(155));
        pingText->setPlainText("999ms");
        return;
    }

    // Set the pingText the appropriate color
    if(ping < 100) pingText->setDefaultTextColor(QColor(0, 155, 0));
    else if(ping < 200) pingText->setDefaultTextColor(QColor(170, 155, 0));
    else pingText->setDefaultTextColor(QColor(155, 0, 0));

    pingText->setPlainText(QString::number(ping) + "ms");
}

void ServerListItem::processUdpPacket(QHostAddress ip, int port, QByteArray data)
{
    QString ipAddress = ip.toString();
    if(ipAddress != "" && ipAddress.at(0) == ':') // if we get IPv6 (::ffff:) we just take the IPv4 part at the end
        ipAddress = ipAddress.mid(7);

    if(ipAddress != serverIP) return; // we only accept our server

    if(Packet::determinePacketType(data) == PacketType::PING)
    {
        PacketPing packet(data);
        if(!packet.isValid())
        {
            qDebug() << packet.getErrorMsg();
            return;
        }
        if(packet.getServerId() != id) return; // If this isn't our packet don't process it
        // Get the time that has elapsed since we last ran it
        ping = pingTimer.elapsed();
    }
}

void ServerListItem::processTcpPacket(QTcpSocket *socket, QByteArray data)
{
    if(socket->peerAddress().toIPv4Address() != QHostAddress(serverIP).toIPv4Address()) return;

    PacketType type = Packet::determinePacketType(data);

    if(type == PacketType::JOIN)
    {
        qDebug() << "joining!";
        PacketJoin packet(data);
        if(!packet.isValid())
        {
            qDebug() << packet.getErrorMsg();
            return;
        }
        if(packet.getUsername() != w->getUsername()) return; // If we get random join that isn't us we just skip (this is for joining!)
        w->getMultiplayerGame()->prepare();

        // Send the server that we want level data
        PacketLevelData levelPacket = PacketLevelData();
        w->getClient()->sendPacket(QHostAddress(serverIP), SERVER_PORT, &levelPacket, Protocol::TCP);
        qDebug() << "You have joined the server!";
    }
    else if(type == PacketType::ERROR)
    {
        PacketError error(data);
        Notification::show("SERVER: " + error.getError(), NotificationType::ERROR);
    }
    else if(type == PacketType::SERVER_SHUTDOWN) // Server has been shut down
    {
        disconnectedState();
    }
}

void ServerListItem::myProcessTcpDatagram()
{   
    QByteArray datagram = myTcpSocket->readAll();

    // When we get the INFO packet from the server the connection is established
    if(Packet::determinePacketType(datagram) == PacketType::INFO)
    {
        PacketInfo infoPacket(datagram);
        if(!infoPacket.isValid())
        {
            qDebug() << infoPacket.getErrorMsg();
            return;
        }

        serverNameText->setPlainText(infoPacket.getServerName());
        serverCapacity->setPlainText(QString::number(infoPacket.getConnectedPlayers()) + "/" + QString::number(infoPacket.getMaxPlayers()));
        isEstablished = true;
        this->disconnect(myTcpSocket, SIGNAL(readyRead()), this, SLOT(myProcessTcpDatagram()));
    }
}

void ServerListItem::disconnectedState()
{
    stopTimers();
    pingText->setDefaultTextColor(QColor(155, 0, 0));
    pingText->setPlainText("-1ms");
    serverCapacity->setPlainText("0/0");
    isEstablished = false;
}

void ServerListItem::connected()
{
    this->connect(myTcpSocket, SIGNAL(readyRead()), this, SLOT(myProcessTcpDatagram()));
    // Send INFO request packet
    PacketInfo infoPacket = PacketInfo();
    myTcpSocket->write(infoPacket.getData(true));
}

void ServerListItem::connecting()
{
    if(isEstablished) // If the server is established stop trying to connect
    {
        connectingTimer->stop();
        // Start the pinging of the server
        this->connect(refreshTimer, SIGNAL(timeout()), this, SLOT(pingServer()));
        refreshTimer->start();

        return;
    }

    // Connecting animation
    QString string = Language::get("connecting");
    if(step == 0) serverNameText->setPlainText(string);
    else if(step == 1) serverNameText->setPlainText(string + ".");
    else if(step == 2) serverNameText->setPlainText(string + "..");
    else serverNameText->setPlainText(string + "...");

    step++;
    if(step == 4)
    {
        // If we are connected send INFO request packet
        if(myTcpSocket->state() == QAbstractSocket::ConnectedState)
        {
            PacketInfo infoPacket = PacketInfo();
            myTcpSocket->write(infoPacket.getData(true));
        }
        step = 0;
    }
}

void ServerListItem::setHighlighted(bool highlighted)
{
    this->highlighted = highlighted;
    this->update();
}

void ServerListItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setHighlighted(true);
    w->getMainMenu()->getMultiplayerPage()->getConnectButton()->setButtonEnabled(true);
    w->getMainMenu()->getMultiplayerPage()->getDeleteButton()->setButtonEnabled(true);
}

void ServerListItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isEstablished) return;
    if(myTcpSocket->state() != QAbstractSocket::ConnectedState) return;

    if(w->getClient()->connectToHandle(myTcpSocket)) // Put the handle to tcp socket in the client
    {
        // Send the join packet
        PacketJoin joinPacket(w->getUsername());
        w->getClient()->sendPacket(QHostAddress(serverIP), SERVER_PORT, &joinPacket, Protocol::TCP);
    }
}

void ServerListItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(highlighted)
    {
        QPainterPath path;
        path.addRect(container->x(), container->y(), container->boundingRect().width(), container->boundingRect().height());
        QPen pen(QColor(125, 125, 0), 10);
        painter->setPen(pen);
        painter->drawPath(path);
    }

    QGraphicsItemGroup::paint(painter, option, widget);
}

void ServerListItem::stopTimers()
{
    connectingTimer->stop();
    refreshTimer->stop();
}

void ServerListItem::resetContent()
{
    w->getClient()->disconnectFromHandle(myTcpSocket);

    this->connect(myTcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    this->connect(myTcpSocket, SIGNAL(disconnected()), this, SLOT(disconnectedState()));

    if(myTcpSocket->state() != QAbstractSocket::ConnectedState &&
       myTcpSocket->state() != QAbstractSocket::ConnectingState)
    {
        // If we aren't connected to server yet we try to
        myTcpSocket->connectToHost(QHostAddress(serverIP), SERVER_PORT); // This is already connected to the connected() SLOT that will connect the readyRead() signal
    }
    else
    {
        this->connect(myTcpSocket, SIGNAL(readyRead()), this, SLOT(myProcessTcpDatagram()));
    }

    // Set the text as though we aren't connected
    disconnectedState();

    step = 0;
    connectingTimer->start();
}
