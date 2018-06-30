#include "packetfirebullet.h"

#include <QStringList>

#include "core/window.h"
#include "game/multiplayergame.h"
#include "network/entities/playermp.h"

extern Window* w;
PacketFireBullet::PacketFireBullet(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketFireBullet::PacketFireBullet(QString username, QPointF pos, int dir) : Packet(PacketType::FIRE_BULLET)
{
    writeFireBullet(username, pos, dir);
}

PacketFireBullet::PacketFireBullet() : Packet(PacketType::FIRE_BULLET)
{
    valid = false;
}

void PacketFireBullet::writeFireBullet(QString username, QPointF pos, int dir)
{
    QString data = "";
    data += username + "," + QString::number(pos.x()) + "," + QString::number(pos.y()) + "," + QString::number(dir);
    this->writeData(data);
}

bool PacketFireBullet::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    // [playerName],[xOfBullet],[yOfBullet],[dirOfBullet]
    if(splitData.length() != 4)
    {
        errorMsg = "FIRE_BULLET packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketFireBullet::execute()
{
    if(!valid) return false;

    PlayerMP* player = w->getMultiplayerGame()->getPlayer(getUsername());
    if(!valid)
    {
        errorMsg = "FIRE_BULLET " + getUsername() + " is not on my list!";
        return false;
    }

    player->fireBullet();

    return true;
}

QString PacketFireBullet::getUsername()
{
    QString username = "";
    if(!valid) return username;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    username = splitData[0];

    return username;
}

QPointF PacketFireBullet::getBulletPos()
{
    QPointF pos(0.0f, 0.0f);
    if(!valid) return pos;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    float x = splitData[1].toFloat();
    float y = splitData[2].toFloat();

    return QPointF(x, y);
}

int PacketFireBullet::getBulletDir()
{
    int dir = 0;
    if(!valid) return dir;

    QString data = this->getData();
    QStringList splitData = data.split(",");
    dir = splitData[3].toInt();

    return dir;
}
