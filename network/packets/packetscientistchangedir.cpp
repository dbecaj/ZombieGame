#include "packetscientistchangedir.h"

#include <QStringList>

#include "game/entities/mobs/mob.h"
#include "core/window.h"
#include "game/multiplayergame.h"
#include "network/entities/scientistmp.h"

extern Window* w;
PacketScientistChangeDir::PacketScientistChangeDir(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketScientistChangeDir::PacketScientistChangeDir(long id, QPointF pos, float dir) : Packet(PacketType::SCIENTIST_CHANGE_DIR)
{
    writeChangeDirection(id, pos, dir);
}

PacketScientistChangeDir::PacketScientistChangeDir() : Packet(PacketType::SCIENTIST_CHANGE_DIR)
{
    valid = false;
}

void PacketScientistChangeDir::writeChangeDirection(long id, QPointF pos, float dir)
{
    QString data = "";
    data = QString::number(id) + "," + QString::number(pos.x()) + "," +
           QString::number(pos.y() - (MOB_HEIGHT - MOB_COLL_HEIGHT)) + "," + QString::number(dir);
    this->writeData(data);
}

bool PacketScientistChangeDir::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");

    //[id],[x],[y],[dir]
    if(splitData.length() != 4)
    {
        errorMsg = "SCIENTIST_CHANGE_DIR packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketScientistChangeDir::execute()
{
    if(!valid) return false;

    ScientistMP* scientist = w->getMultiplayerGame()->getScientist(getScientistId());
    if(!scientist)
    {
        errorMsg = "SCIENTIST_CHANGE_DIR I don't have a scientist on my list with the id " +
                   QString::number(getScientistId());
        return false;
    }

    scientist->setPos(getScientistPos());
    scientist->changeAngle(getScientistDir());

    return true;
}

long PacketScientistChangeDir::getScientistId()
{
    long id = -1;
    if(!valid) return id;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    id = splitData[0].toLong();

    return id;
}

QPointF PacketScientistChangeDir::getScientistPos()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat() - (MOB_HEIGHT - MOB_COLL_HEIGHT); // transform from collision pos to bounding box

    return QPointF(x, y);
}

float PacketScientistChangeDir::getScientistDir()
{
    float dir = 0.0f;
    if(!valid) return dir;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    dir = splitData[3].toFloat();

    return dir;
}
