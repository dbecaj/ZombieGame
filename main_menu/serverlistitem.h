#ifndef SERVERLISTITEM_H
#define SERVERLISTITEM_H

#include <QObject>
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QTcpSocket>

#include "listitem.h"
#include "network/packets/packet.h"

#define SERVERLISTITEM_WIDTH 500.0f
#define SERVERLISTITEM_HEIGHT 150.0f

class QGraphicsTextItem;

class ServerListItem : public QObject, public ListItem
{
    Q_OBJECT

    long id; // We have this so that the server knows which serverItem is requesting something
    QString serverIP;
    QString serverName;

    int step; // This is used only for animation
    bool isEstablished; // This tells us if the we verified that the server connection is OK
    QGraphicsTextItem* pingText;
    QGraphicsTextItem* serverNameText;
    QGraphicsTextItem* serverIPText;
    QGraphicsTextItem* serverCapacity;
    QTimer* connectingTimer;
    QTimer* refreshTimer;
    QElapsedTimer pingTimer; // This measures our current ping (although not accurate)
    int ping; // This stores the measured ping
    QTcpSocket* myTcpSocket; // This process data for as long as we don't pass it to Client
    bool highlighted;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private slots:
    void pingServer(); // Sends PING packet
    void processUdpPacket(QHostAddress ip, int port, QByteArray data);
    void processTcpPacket(QTcpSocket* socket, QByteArray data);
    void myProcessTcpDatagram(); // This is just for the basic info about the server then we break it when we pass the handle
    void connecting(); // Trys to connect to the server and sends the empty INFO packet each second
    void connected(); // Is called when we established the server connection with the tcpServer
    void disconnectedState(); // Stops the timers and shows the server as disconnected (-1ms)
public:
    ServerListItem(float x, float y, QString serverIP, long id, QGraphicsItem *parent = nullptr);
    ~ServerListItem();

    void stopTimers();
    void resetContent(); // Disconnects the handle from the Client(if it has it) then just establishes the myTcpSocket to be the main socket

    QTcpSocket* getTcpSocket() { return myTcpSocket; }
    QString getServerIP() { return serverIP; }
    void setHighlighted(bool highlighted);
    bool isHighlighted() { return highlighted; }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); // This is when we give the handle to TCP socket(myTcpSocket) to Client (Trys to join the clicked server)
};

#endif // SERVERLISTITEM_H
