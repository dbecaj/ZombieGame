#include "packetplayerpos.h"

#include <QStringList>

#include "game/entities/mobs/mob.h"
#include "core/window.h"
#include "game/multiplayergame.h"
#include "network/entities/playermp.h"

extern Window* w;
PacketPlayerPos::PacketPlayerPos(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketPlayerPos::PacketPlayerPos(QString username, QPointF pos, float degrees) : Packet(PacketType::PLAYER_POS)
{
    writePlayerPos(username, pos, degrees);
}

PacketPlayerPos::PacketPlayerPos() : Packet(PacketType::PLAYER_POS)
{
    valid = false;
}

bool PacketPlayerPos::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    if(splitData.length() != 4) // [playerName],[x],[y],[degrees]
    {
        errorMsg = "PLAYER_POS packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

void PacketPlayerPos::writePlayerPos(QString username, QPointF pos, float degrees)
{
    float x = pos.x();
    float y = pos.y() + (MOB_HEIGHT - MOB_COLL_HEIGHT); // we send the collision box pos
    QString data = username + "," + QString::number(x) + "," + QString::number(y) + "," + QString::number(degrees);
    this->writeData(data);
}

bool PacketPlayerPos::execute()
{
    if(!valid) return false;

    PlayerMP* player = w->getMultiplayerGame()->getPlayer(getUsername());
    if(!player)
    {
        errorMsg = "PLAYER POS " + getUsername() + " is not on my list!";
        return false;
    }

    player->updatePosition(getPosition(), getDegrees());

    return true;
}

QString PacketPlayerPos::getUsername()
{
    QString username = "";
    if(!valid) return username;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    username = splitData[0];

    return username;
}

QPointF PacketPlayerPos::getPosition()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat() - (MOB_HEIGHT - MOB_COLL_HEIGHT); // transform to bounding box pos

    return QPointF(x, y);
}

float PacketPlayerPos::getDegrees()
{
    float degrees = 0.0f;
    if(!valid) return degrees;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    degrees = splitData[3].toFloat();

    return degrees;
}
