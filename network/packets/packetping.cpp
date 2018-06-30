#include "packetping.h"

PacketPing::PacketPing(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketPing::PacketPing(long serverId) : Packet(PacketType::PING)
{
    writePing(serverId);
}

PacketPing::PacketPing() : Packet(PacketType::PING)
{
    valid = false;
}

void PacketPing::writePing(long serverId)
{
    QString idServer = "";
    if(serverId < 9) idServer += "0";
    idServer += QString::number(serverId);

    this->writeData(idServer);
}

bool PacketPing::validate()
{
    valid = false;

    if(getData().contains(','))
    {
        errorMsg = "PING packet error: The packet contains a ','!";
        return false;
    }
    bool ok = false;
    long id = this->getData().toLong(&ok);
    if(!ok)
    {
        errorMsg  = "PING packet error: packet data to long conversion went wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketPing::execute()
{
    if(!valid) return false;

    return true;
}

long PacketPing::getServerId()
{
    if(!valid) return -1;

    return getData().toLong();
}
