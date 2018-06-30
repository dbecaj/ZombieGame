#include "packetjoin.h"

#include "core/window.h"
#include "game/multiplayergame.h"
#include "network/entities/playermp.h"

extern Window* w;
PacketJoin::PacketJoin(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketJoin::PacketJoin(QString username) : Packet(PacketType::JOIN)
{
    writeJoin(username);
}

PacketJoin::PacketJoin() : Packet(PacketType::JOIN)
{
    valid = false;
}

void PacketJoin::writeJoin(QString username)
{
    this->writeData(username);
}

bool PacketJoin::validate()
{
    valid = false;

    if(getData().contains(','))
    {
        errorMsg = "JOIN packet error: The packet contains ','!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketJoin::execute()
{
    if(!valid) return false;

    if(getUsername() == w->getUsername())
    {
        errorMsg = "JOIN packet error: The username is the same as yours!";
        return false;
    }

    qDebug() << "adding player!";
    PlayerMP* player = new PlayerMP(w->getMultiplayerGame()->getLevel()->playerStartPos,
                                    getUsername());
    w->getMultiplayerGame()->addPlayer(player);
    w->getMultiplayerGame()->getChatBox()->addText(getUsername() + " has joined!",
                                                   QColor(34, 139, 34));
    qDebug() << player->getUsername() << "added!";

    return true;
}

QString PacketJoin::getUsername()
{
    if(!valid) return "";

    return getData();
}
