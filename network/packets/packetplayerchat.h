#ifndef PACKETPLAYERCHAT_H
#define PACKETPLAYERCHAT_H

#include <QString>

#include "packet.h"

class PacketPlayerChat : public Packet
{
public:
    PacketPlayerChat(QByteArray data);
    PacketPlayerChat(QString username, QString message);
    PacketPlayerChat();

    void writePlayerChatData(QString message, QString username);
    bool validate();
    bool execute();

    QString getUsername();
    QString getMessage();
};

#endif // PACKETPLAYERCHAT_H
