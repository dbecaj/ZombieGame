#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QWidget>

#include "network/packets/packet.h"

#define CLIENT_PORT 6337
#define SERVER_PORT 6336

enum class Protocol
{
    TCP,
    UDP
};

class Client : public QWidget
{
    Q_OBJECT

    QUdpSocket* udpSocket;
    QTcpSocket* tcpSocket;
    QList<QByteArray> receiveQueue;

    void processTcpData(QByteArray data);
private slots:
    void processUdpPendingDatagrams();
    void processTcpPendingDatagrams();
signals:
    void udpPacketReceived(QHostAddress ip, int port, QByteArray data); // For sending the data down the line
    void tcpPacketReceived(QTcpSocket* socket, QByteArray data); // Same here
public:
    Client();
    ~Client();

    void bindUdp(int port);
    bool connectTcp(QHostAddress ip, int port);
    bool connectToHandle(QTcpSocket* handle); // Sets the tcpSocket pointer to handle if the handle is connected
    bool closeTcp(); // Disconnects from the tcpServer
    bool disconnectFromHandle(); // Just calls disconnectFromHandle(tcpSocket) should be no problems
    bool disconnectFromHandle(QTcpSocket* handle); // Disconnects the readReady() from the tcpSocket and sets the tcpSocket = nullptr if the handle matches the tcpSocket pointer
    void sendMessage(QHostAddress ip, int port, QByteArray datagram, Protocol protocol); // Primitive way to just send raw data to server
    void sendPacket(QHostAddress ip, int port, Packet* packet, Protocol protocol); // Send packet to server(if the Protocol is TCP the ip and port are redundant)
    QUdpSocket* getUdpSocket() { return udpSocket; }
    QTcpSocket* getTcpSocket() { return tcpSocket; }
};

#endif // CLIENT_H
