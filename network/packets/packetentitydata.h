#ifndef PACKETENTITYDATA_H
#define PACKETENTITYDATA_H

#include <QList>

#include "packet.h"
#include "network/entities/playermp.h"
#include "network/entities/scientistmp.h"
#include "network/entities/zombiemp.h"

class PacketEntityData : public Packet
{
    QString usernames;
    QString zombies;
    QString scientists;
public:
    PacketEntityData(QByteArray data);
    PacketEntityData(QList<PlayerMP*> players, QList<ZombieMP*> zombies, QList<ScientistMP*> scientists);
    PacketEntityData();

    void writeEntityData(QList<PlayerMP*> players, QList<ZombieMP*> zombies, QList<ScientistMP*> scientists);
    bool validate();
    bool execute();

    QList<PlayerMP*> getPlayers();
    QList<ZombieMP*> getZombies();
    QList<ScientistMP*> getScientists();
};

#endif // PACKETENTITYDATA_H
