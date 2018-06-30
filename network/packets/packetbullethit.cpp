#include "packetbullethit.h"

#include <QStringList>
#include <QDebug>

#include "core/window.h"
#include "network/entities/zombiemp.h"
#include "game/multiplayergame.h"

extern Window* w;
PacketBulletHit::PacketBulletHit(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketBulletHit::PacketBulletHit(long zombieId, int lives, QString username) : Packet(PacketType::BULLET_HIT)
{
    writeBulletHitData(zombieId, lives, username);
}

PacketBulletHit::PacketBulletHit() : Packet(PacketType::BULLET_HIT)
{
    valid = false;
}

void PacketBulletHit::writeBulletHitData(long zombieId, int lives, QString username)
{
    QString data = "";
    data += QString::number(zombieId) + "/" + QString::number(lives) + "," + username;

    this->writeData(data);
}

bool PacketBulletHit::validate()
{
    valid = false;
    QString data = this->getData();
    QStringList splitData = data.split(",");

    // [idZombie]/[lives],[playerName]
    if(splitData.length() != 2)
    {
        errorMsg = "BULLET_HIT packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketBulletHit::execute()
{
    if(!valid) return false;

    ZombieMP* zombie = w->getMultiplayerGame()->getZombie(getZombieId());

    if(!zombie)
    {
        errorMsg = "BULLET_HIT packet error: I don't have a zombie with id " +
                    QString::number(getZombieId());
        return false;
    }

    zombie->setLives(getLives());
    zombie->playAttackedSound();

    if(zombie->getLives() == 0)
    {
        if(!w->getMultiplayerGame()->removeZombie(zombie))
        {
            errorMsg = "BULLET_HIT Could not delete the zombie!";
            return false;
        }
    }

    return true;
}

long PacketBulletHit::getZombieId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    id = splitData[0].split("/")[0].toLong();
    return id;
}

QString PacketBulletHit::getUsername()
{
    QString username = "";
    if(!valid) return username;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    username = splitData[1];

    return username;
}

int PacketBulletHit::getLives()
{
    int lives = -1;
    if(!valid) return lives;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    lives = splitData[0].split("/").last().toInt();

    return lives;
}
