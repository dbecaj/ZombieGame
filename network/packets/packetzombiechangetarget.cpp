#include "packetzombiechangetarget.h"

#include "game/entities/mobs/mob.h"
#include "network/entities/playermp.h"
#include "network/entities/scientistmp.h"
#include "game/multiplayergame.h"
#include "core/window.h"

extern Window* w;
PacketZombieChangeTarget::PacketZombieChangeTarget(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketZombieChangeTarget::PacketZombieChangeTarget(long zombieId, QPointF zombiePos, Mob *target) : Packet(PacketType::ZOMBIE_CHANGE_TARGET)
{
    writeChangeTarget(zombieId, zombiePos, target);
}

PacketZombieChangeTarget::PacketZombieChangeTarget() : Packet(PacketType::ZOMBIE_CHANGE_TARGET)
{
    valid = false;
}

void PacketZombieChangeTarget::writeChangeTarget(long zombieId, QPointF zombiePos, Mob *target)
{
    QString data = "";
    data += QString::number(zombieId) + "," + QString::number(zombiePos.x()) + "," + QString::number(zombiePos.y() + (MOB_HEIGHT - MOB_COLL_HEIGHT)) + ","; // transform to collision box pos
    if(target->getMobType() == MobType::PLAYER)
    {
        PlayerMP* player = (PlayerMP*)target;
        data += "PLAYER," + player->getUsername();
    }
    else if(target->getMobType() == MobType::SCIENTIST)
    {
        ScientistMP* scientist = (ScientistMP*)target;
        data += "SCIENTIST," + QString::number(scientist->getId());
    }

    this->writeData(data);
}

bool PacketZombieChangeTarget::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    // [id],[x],[y],[mobType],[idTarget/playerName]
    if(splitData.length() != 5)
    {
        errorMsg = "ZOMBIE_CHANGE_TARGET packet error: The packet format is wrong! " + QString::number(splitData.length());
        return false;
    }

    valid = true;
    return true;
}

bool PacketZombieChangeTarget::execute()
{
    if(!valid) return false;

    ZombieMP* zombie = w->getMultiplayerGame()->getZombie(getZombieId());
    if(!zombie)
    {
        errorMsg = "ZOMBIE_CHANGE_TARGET I don't have zombie with the id " + QString::number(getZombieId()) +
                   " on my list!";
        return false;
    }

    zombie->setPos(getZombiePosition());

    Mob* target = getTarget();
    if(!target)
    {
        errorMsg = "ZOMBIE_CHANGE_TARGET I don't have player/scientist with the id " + getTargetId() +
                   " on my list!";
        return false;
    }

    zombie->setTarget(target);

    return true;
}

long PacketZombieChangeTarget::getZombieId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[0].toLong();

    return id;
}

QPointF PacketZombieChangeTarget::getZombiePosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat()  - (MOB_HEIGHT - MOB_COLL_HEIGHT);

    return QPointF(x, y);
}

Mob* PacketZombieChangeTarget::getTarget()
{
    if(!valid) return nullptr;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData[3] == "PLAYER")
    {
        PlayerMP* player = w->getMultiplayerGame()->getPlayer(splitData[4]);
        return player;
    }
    else if(splitData[3] == "SCIENTIST")
    {
        ScientistMP* scientist = w->getMultiplayerGame()->getScientist(splitData[4].toLong());
        return scientist;
    }
}

QString PacketZombieChangeTarget::getTargetId()
{
    QString id = "";
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[4];

    return id;
}
