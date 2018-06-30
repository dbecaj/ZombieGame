#ifndef PACKETPING_H
#define PACKETPING_H

#include "packet.h"

class PacketPing : public Packet
{
public:
    PacketPing(QByteArray data);
    PacketPing(long serverId);
    PacketPing();

    void writePing(long serverId);
    bool validate();
    bool execute();

    long getServerId();
};

#endif // PACKETPING_H
