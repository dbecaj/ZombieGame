#ifndef PACKETZOMBIEHIT_H
#define PACKETZOMBIEHIT_H

#include "packet.h"
#include "network/entities/zombiemp.h"

class PacketZombieHit : public Packet
{
public:
    PacketZombieHit(QByteArray data);
    PacketZombieHit(ZombieMP* zombie, Mob* target, long newZombieId);
    PacketZombieHit();

    void writeZombieHit(ZombieMP* zombie, Mob* target, long newZombieId);
    bool validate();
    bool execute();

    long getZombieId();
    long getNewZombieId();
    QPointF getZombiePosition();
    Mob* getHitTarget();
    QPointF getHitTargetPosition();
};

#endif // PACKETZOMBIEHIT_H
