#ifndef PACKETBULLETHIT_H
#define PACKETBULLETHIT_H

#include <QString>

#include "packet.h"

class PacketBulletHit : public Packet
{
public:
    PacketBulletHit(QByteArray data);
    PacketBulletHit(long zombieId, int lives, QString username);
    PacketBulletHit();

    void writeBulletHitData(long zombieId, int lives, QString username);

    bool validate();
    bool execute();

    long getZombieId();
    QString getUsername();
    int getLives();
};

#endif // PACKETBULLETHIT_H
