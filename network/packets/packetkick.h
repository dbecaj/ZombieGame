#ifndef PACKETKICK_H
#define PACKETKICK_H

#include "packet.h"

class PacketKick : public Packet
{
public:
    PacketKick(QByteArray data);
    PacketKick(QString username);
    PacketKick();

    void writeKick(QString username);
    bool validate();
    bool execute();

    QString getUsername();
};

#endif // PACKETKICK_H
