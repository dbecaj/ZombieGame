#ifndef PACKETDISCONNECT_H
#define PACKETDISCONNECT_H

#include "packet.h"

class PacketDisconnect : public Packet
{
public:
    PacketDisconnect(QByteArray data);
    PacketDisconnect(QString username);
    PacketDisconnect();

    void writeDisconnect(QString username);
    bool validate();
    bool execute();

    QString getUsername();
};

#endif // PACKETDISCONNECT_H
