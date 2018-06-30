#include "packetzombiehit.h"

#include <QStringList>

#include "network/entities/playermp.h"
#include "network/entities/scientistmp.h"
#include "core/window.h"
#include "game/multiplayergame.h"

extern Window* w;
PacketZombieHit::PacketZombieHit(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketZombieHit::PacketZombieHit(ZombieMP *zombie, Mob *target, long newZombieId) : Packet(PacketType::ZOMBIE_HIT)
{
    writeZombieHit(zombie, target, newZombieId);
}

PacketZombieHit::PacketZombieHit() : Packet(PacketType::ZOMBIE_HIT)
{
    valid = false;
}

void PacketZombieHit::writeZombieHit(ZombieMP *zombie, Mob *target, long newZombieId)
{
    QString data = "";
    data += QString::number(zombie->getId()) + "," + QString::number(zombie->pos().x()) + "," + QString::number(zombie->pos().y());
    data += "," + QString::number(target->pos().x()) + "," + QString::number(target->pos().y());
    if(target->getMobType() == MobType::PLAYER)
    {
        PlayerMP* player = (PlayerMP*)target;
        data += "," + QString::fromStdString("PLAYER") + "," + player->getUsername();
    }
    else if(target->getMobType() == MobType::SCIENTIST)
    {
        Scientist* scientist = (Scientist*)target;
        data += "," + QString::fromStdString("SCIENTIST") + "," + QString::number(scientist->getId());
    }
    data += "," + QString::number(newZombieId);

    this->writeData(data);
}

bool PacketZombieHit::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    // [idZombie],[zombieX],[zombieY],[targetX],[targetY],[mobType],[idTarget/playerName]
    if(splitData.length() != 8)
    {
        errorMsg = "ZOMBIE_HIT packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketZombieHit::execute()
{
    if(!valid) return false;

    ZombieMP* zombie = w->getMultiplayerGame()->getZombie(getZombieId());
    if(!zombie)
    {
        errorMsg = "ZOMBIE_HIT packet error: I don't have a zombie with the id " + QString::number(getZombieId());
        return false;
    }

    zombie->setPos(getZombiePosition());

    Mob* hitTarget = getHitTarget();
    if(!hitTarget)
    {
        errorMsg = "ZOMBIE_HIT packet error: I dont have a mob with id " + QString::number(getZombieId());
        return false;
    }

    if(hitTarget->getMobType() == MobType::SCIENTIST)
    {
        ZombieMP* newZombie = new ZombieMP(getHitTargetPosition(), getNewZombieId());
        w->getMultiplayerGame()->addZombie(newZombie);

        if(!w->getMultiplayerGame()->removeScientist((ScientistMP*)hitTarget))
        {
            errorMsg = "ZOMBIE_HIT cannot removeScientist()!";
            return false;
        }
    }

    return true;
}

long PacketZombieHit::getZombieId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[0].toLong();

    return id;
}

long PacketZombieHit::getNewZombieId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[7].toLong();

    return id;
}

QPointF PacketZombieHit::getZombiePosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat();

    return QPointF(x, y + (MOB_HEIGHT - MOB_COLL_HEIGHT));
}

Mob* PacketZombieHit::getHitTarget()
{
    if(!valid) return nullptr;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData[5] == "PLAYER")
    {
        PlayerMP* player = w->getMultiplayerGame()->getPlayer(splitData[6]);
        return player;
    }
    else if(splitData[5] == "SCIENTIST")
    {
        ScientistMP* scientist = w->getMultiplayerGame()->getScientist(splitData[6].toLong());
        return scientist;
    }

    return nullptr;
}

QPointF PacketZombieHit::getHitTargetPosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[3].toFloat();
    float y = splitData[4].toFloat();

    return QPointF(x, y + (MOB_HEIGHT - MOB_COLL_HEIGHT));
}
