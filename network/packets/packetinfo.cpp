#include "packetinfo.h"

#include <QStringList>

PacketInfo::PacketInfo(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketInfo::PacketInfo(QString serverName, long connectedPlayers, long maxPlayers) : Packet(PacketType::INFO)
{
    writeInfo(serverName, connectedPlayers, maxPlayers);
}

PacketInfo::PacketInfo() : Packet(PacketType::INFO)
{
    valid = false;
}

void PacketInfo::writeInfo(QString serverName, long connectedPlayers, long maxPlayers)
{
    QString data = "";
    data += serverName + "," + QString::number(connectedPlayers) + "," + QString::number(maxPlayers);

    this->writeData(data);
}

bool PacketInfo::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    // [serverName],[numberOfConnectedPlayers],[numberOfMaxPlayers]
    if(splitData.length() != 3)
    {
        errorMsg = "INFO packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketInfo::execute()
{
    return true;
}

QString PacketInfo::getServerName()
{
    QString serverName = "";
    if(!valid) return serverName;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    serverName = splitData[0];

    return serverName;
}

long PacketInfo::getConnectedPlayers()
{
    long connectedPlayers = -1;
    if(!valid) return connectedPlayers;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    connectedPlayers = splitData[1].toInt();

    return connectedPlayers;
}

long PacketInfo::getMaxPlayers()
{
    long maxPlayers = -1;
    if(!valid) return maxPlayers;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    maxPlayers = splitData[2].toInt();

    return maxPlayers;
}
