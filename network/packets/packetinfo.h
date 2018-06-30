#ifndef PACKETINFO_H
#define PACKETINFO_H

#include "packet.h"

class PacketInfo : public Packet
{
public:
    PacketInfo();
    PacketInfo(QByteArray data);
    PacketInfo(QString serverName, long connectedPlayers, long maxPlayers);

    void writeInfo(QString serverName, long connectedPlayers, long maxPlayers);
    bool validate();
    bool execute();

    QString getServerName();
    long getConnectedPlayers();
    long getMaxPlayers();
};

#endif // PACKETINFO_H
