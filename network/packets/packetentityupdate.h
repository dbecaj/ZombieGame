#ifndef PACKETENTITYUPDATE_H
#define PACKETENTITYUPDATE_H

#include "packet.h"

class ZombieMP;
class ScientistMP;

class PacketEntityUpdate : public Packet
{
public:
    PacketEntityUpdate(QByteArray data);
    PacketEntityUpdate(QList<ZombieMP*> zombies, QList<ScientistMP*> scientists);
    PacketEntityUpdate();

    void writeEntityUpdate(QList<ZombieMP*> zombies, QList<ScientistMP*> scientists);
    bool validate();
    bool execute();
};

#endif // PACKETENTITYUPDATE_H
