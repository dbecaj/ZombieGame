#include "packetdisconnect.h"

#include "core/window.h"
#include "game/multiplayergame.h"

extern Window* w;
PacketDisconnect::PacketDisconnect(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketDisconnect::PacketDisconnect(QString username) : Packet(PacketType::DISCONNECT)
{
    writeDisconnect(username);
}

PacketDisconnect::PacketDisconnect() : Packet(PacketType::DISCONNECT)
{
    valid = false;
}

void PacketDisconnect::writeDisconnect(QString username)
{
    this->writeData(username);
}

bool PacketDisconnect::validate()
{
    if(data.contains(','))
    {
        errorMsg = "DISCONNECT packet error: The packet contains ','!";
        return false;
    }

    return true;
}

bool PacketDisconnect::execute()
{
    if(!valid) return false;

    if(getUsername() == w->getUsername())
    {
        errorMsg = "DISCONNECT packet error: The username is yours!";
        return false;
    }

    w->getMultiplayerGame()->removePlayer(getUsername());
    w->getMultiplayerGame()->getChatBox()->addText(getUsername() + " has disconnected!",
                                                   QColor(125, 0, 0));

    return true;
}

QString PacketDisconnect::getUsername()
{
    if(!valid) return "";

    return getData();
}
