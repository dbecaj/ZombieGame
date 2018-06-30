#ifndef PACKETINFECTED_H
#define PACKETINFECTED_H

#include "packet.h"

class PacketInfected : public Packet
{
public:
    PacketInfected(QByteArray data);
    PacketInfected(QString playerName, long zombieId);
    PacketInfected();

    void writeKilled(QString playerName, long zombieId);
    bool validate();
    bool execute();

    QString getUsername();
    long getZombieId();
};

#endif // PACKETINFECTED_H
