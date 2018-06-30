#ifndef PACKETSCIENTISTCHANGEDIR_H
#define PACKETSCIENTISTCHANGEDIR_H

#include <QPointF>
#include <QString>

#include "packet.h"

class PacketScientistChangeDir : public Packet
{
public:
    PacketScientistChangeDir(QByteArray data);
    PacketScientistChangeDir(long id, QPointF pos, float dir);
    PacketScientistChangeDir();

    void writeChangeDirection(long id, QPointF pos, float dir);
    bool validate();
    bool execute();

    long getScientistId();
    QPointF getScientistPos();
    float getScientistDir();
};

#endif // PACKETSCIENTISTCHANGEDIR_H
