#include "client.h"

#include <QDebug>

#include "core/window.h"
#include "network/packets/packeterror.h"
#include "network/packets/packetping.h"

extern Window* w;
Client::Client()
{
    udpSocket = new QUdpSocket();
    tcpSocket = nullptr;
}

Client::~Client()
{
    udpSocket->close();
    delete udpSocket;
    if(tcpSocket)
    {
        tcpSocket->close();
        delete tcpSocket; // Maybe wrong if connectTcp(QHostAddress ip, int port) is called :/ ?
    }
}

void Client::bindUdp(int port)
{
    udpSocket->bind(port, QUdpSocket::ShareAddress);
    this->connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processUdpPendingDatagrams()));
}

bool Client::connectTcp(QHostAddress ip, int port)
{
    if(!tcpSocket)
    {
        tcpSocket = new QTcpSocket();
    }
    tcpSocket->connectToHost(ip, port);
    if(tcpSocket->waitForConnected(2000)) // Try to connect for 2 sec
    {
        this->connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(processTcpPendingDatagrams()));
        this->connect(tcpSocket, SIGNAL(readChannelFinished()), this, SLOT(readNext()));
        return true;
    }

    qDebug() << "TCP socket failed to connect!";

    return false;
}

bool Client::closeTcp()
{
    // If the tcpSocket isn't connected or event doesn't exist return false
    if(!tcpSocket) return false;
    if(tcpSocket->state() != QAbstractSocket::ConnectedState) return false;

    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    return true;
}

bool Client::disconnectFromHandle()
{
    return disconnectFromHandle(tcpSocket);
}

bool Client::disconnectFromHandle(QTcpSocket* handle)
{
    if(tcpSocket != handle) return false;

    tcpSocket->disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(processTcpPendingDatagrams()));
    tcpSocket = nullptr;
    return true;
}

bool Client::connectToHandle(QTcpSocket *handle)
{
    if(!handle) return false;
    if(handle == tcpSocket) return false;

    if(handle->state() == QTcpSocket::SocketState::ConnectedState)
    {
        tcpSocket = handle;
        this->connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(processTcpPendingDatagrams()));
        return true;
    }

    return false;
}

void Client::processUdpPendingDatagrams()
{   
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress ip;
        quint16 port;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &ip, &port);

        PacketType type = Packet::determinePacketType(datagram);

        // Process basic messages others just send down the line
        if(Packet::determinePacketType(datagram) == PacketType::INVALID)
        {
            qDebug() << "UDP:invalid packet!";
        }
        else if(Packet::determinePacketType(datagram) == PacketType::ERROR)
        {
            PacketError packet(datagram);
            qDebug() << "UDP:ERROR: " << packet.getError();
        }
        else if(Packet::determinePacketType(datagram) == PacketType::BASIC)
        {
            QString ipAddress = ip.toString();
            if(ipAddress != "" && ipAddress.at(0) == ':') // if we get IPv6 (::ffff:) we just take the IPv4 part at the end
                ipAddress = ipAddress.mid(7);

            qDebug("SERVER:UDP[" + ipAddress.toUtf8() + ":" + QString::number(port).toUtf8() + "] > " + Packet::extractData(datagram).toUtf8());
        }
        else
        {
            emit udpPacketReceived(ip, port, datagram); // Sending packet for other who might want it :3
        }
    }
}

void Client::processTcpPendingDatagrams()
{
    static quint32 packetSize = 0;
    static QByteArray dataStore;

    QByteArray data = tcpSocket->readAll();
    while(data.length() != 0)
    {
        if(packetSize <= 0) packetSize = Packet::determinePacketSize(data) + 4;
        if(data.length() >= packetSize) // The packetSize in header is 4 bytes long
        {
            processTcpData(data.mid(0, packetSize));
            data = data.mid(packetSize);
            packetSize = 0;
        }
        else
        {
            dataStore.append(data);
            packetSize -= data.length();
            if(packetSize <= 0) processTcpData(dataStore);
            data.clear();
        }
    }
}

void Client::processTcpData(QByteArray data)
{
    PacketType type = Packet::determinePacketType(data);

    // Process basic messages others just send down the line
    if(type == PacketType::INVALID)
    {
        qDebug() << "TCP:invalid packet!";
    }
    else if(type == PacketType::BASIC)
    {
        QHostAddress ip = tcpSocket->peerAddress();
        int port = tcpSocket->peerPort();

        QString ipAddress = ip.toString();
        if(ipAddress != "" && ipAddress.at(0) == ':') // if we get IPv6 (::ffff:) we just take the IPv4 part at the end
            ipAddress = ipAddress.mid(7);

        qDebug("SERVER:TCP[" + ipAddress.toUtf8() + ":" + QString::number(port).toUtf8() + "] > " + Packet::extractData(data).toUtf8());
    }
    else
    {
        emit tcpPacketReceived(tcpSocket, data);
    }
}

void Client::sendPacket(QHostAddress ip, int port, Packet *packet, Protocol protocol)
{
    sendMessage(ip, port, packet->getData(true), protocol);
}

void Client::sendMessage(QHostAddress ip, int port, QByteArray datagram, Protocol protocol)
{
    if(protocol == Protocol::TCP)
    {
        if(tcpSocket->state() != QAbstractSocket::ConnectedState) return;
        if(tcpSocket->write(datagram) == -1) qDebug() << "TCP: Error while sending datagram";
    }
    else
    {
        if(udpSocket->writeDatagram(datagram, ip, port) == -1) qDebug() << "UDP: Error while sending datagram";
    }
}
