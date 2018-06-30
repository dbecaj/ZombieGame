#include "packetinfected.h"

#include <QStringList>
#include <QDebug>

#include "core/window.h"
#include "network/entities/playermp.h"
#include "game/multiplayergame.h"

extern Window* w;
PacketInfected::PacketInfected(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketInfected::PacketInfected(QString playerName, long zombieId) : Packet(PacketType::INFECTED)
{
    writeKilled(playerName, zombieId);
}

PacketInfected::PacketInfected() : Packet(PacketType::INFECTED)
{
    valid = false;
}

void PacketInfected::writeKilled(QString playerName, long zombieId)
{
    this->writeData(playerName + "," + QString::number(zombieId));
}

bool PacketInfected::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData.length() != 2)
    {
        errorMsg = "INFECTED packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketInfected::execute()
{
    if(!valid) return false;

    PlayerMP* player = w->getMultiplayerGame()->getPlayer(getUsername());
    if(!player)
    {
        errorMsg = "INFECTED Can't find the player " + getUsername();
        return false;
    }

    ZombieMP* zombie = new ZombieMP(player->pos(), getZombieId());

    w->getMultiplayerGame()->addZombie(zombie);
    w->getMultiplayerGame()->removePlayer(getUsername());

    if(getUsername() == w->getUsername())
        w->getMultiplayerGame()->getChatBox()->addText
        ("You have become infected!", QColor(0, 125, 0));
    else
    {
        w->getMultiplayerGame()->getChatBox()->addText
        (getUsername() + " has been infected!", QColor(0, 125, 0));
    }

    return true;
}

QString PacketInfected::getUsername()
{
    if(!valid) return "";

    QString data = this->getData();
    QStringList splitData = data.split(",");
    return splitData[0];
}

long PacketInfected::getZombieId()
{
    if(!valid) return -1;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    return splitData[1].toLong();
}
