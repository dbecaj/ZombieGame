#include "packetkick.h"

#include "core/window.h"
#include "game/multiplayergame.h"
#include "misc/notification.h"

extern Window* w;
PacketKick::PacketKick(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketKick::PacketKick(QString username) : Packet(PacketType::KICK)
{
    writeKick(username);
}

PacketKick::PacketKick() : Packet(PacketType::KICK)
{
    valid = false;
}

void PacketKick::writeKick(QString username)
{
    this->writeData(username);
}

bool PacketKick::validate()
{
    valid = false;

    if(data.contains(','))
    {
        errorMsg = "KICK packet error: The packet contains a ','!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketKick::execute()
{
    if(!valid) return false;

    if(getUsername() == w->getUsername())
    {
        Notification::show("You have been kicked!", NotificationType::ALERT);
        w->getMultiplayerGame()->quitGame();
    }
    else
    {
        w->getMultiplayerGame()->removePlayer(getUsername());
        w->getMultiplayerGame()->getChatBox()->addText(getUsername() + " has been kicked!",
                                                       QColor(125, 0, 0));
    }

    return true;
}

QString PacketKick::getUsername()
{
    if(!valid) return "";

    return this->getData();
}
