#ifndef PACKETZOMBIECHANGETARGET_H
#define PACKETZOMBIECHANGETARGET_H

#include <QPointF>

#include "packet.h"
#include "game/entities/mobs/mob.h"

class PacketZombieChangeTarget : public Packet
{
public:
    PacketZombieChangeTarget(QByteArray data);
    PacketZombieChangeTarget(long zombieId, QPointF zombiePos, Mob* target);
    PacketZombieChangeTarget();

    void writeChangeTarget(long zombieId, QPointF zombiePos, Mob* target);
    bool validate();
    bool execute();

    long getZombieId();
    QPointF getZombiePosition();
    Mob* getTarget();
    QString getTargetId();
};

#endif // PACKETZOMBIECHANGETARGET_H
