#ifndef PACKETERROR_H
#define PACKETERROR_H

#include "packet.h"

class PacketError : public Packet
{
public:
    PacketError(QByteArray data);
    PacketError(QString errorMsg);
    PacketError();

    void writeError(QString error);
    bool validate();
    bool execute();

    QString getError();
};

#endif // PACKETERROR_H
