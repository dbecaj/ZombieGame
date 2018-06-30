#ifndef PACKETPLAYERPOS_H
#define PACKETPLAYERPOS_H

#include <QPointF>
#include <QString>

#include "packet.h"

class PacketPlayerPos : public Packet
{
public:
    PacketPlayerPos(QByteArray data);
    PacketPlayerPos(QString username, QPointF pos, float degrees);
    PacketPlayerPos();

    void writePlayerPos(QString username, QPointF pos, float degrees);
    bool validate();
    bool execute();

    QString getUsername();
    QPointF getPosition();
    float getDegrees();
};

#endif // PACKETPLAYERPOS_H
