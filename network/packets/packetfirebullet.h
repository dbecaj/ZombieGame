#ifndef PACKETFIREBULLET_H
#define PACKETFIREBULLET_H

#include <QPointF>
#include <QString>

#include "packet.h"

class PacketFireBullet : public Packet
{
public:
    PacketFireBullet(QByteArray data);
    PacketFireBullet(QString username, QPointF pos, int dir);
    PacketFireBullet();

    void writeFireBullet(QString username, QPointF pos, int dir);
    bool validate();
    bool execute();

    QString getUsername();
    QPointF getBulletPos();
    int getBulletDir();
};

#endif // PACKETFIREBULLET_H
