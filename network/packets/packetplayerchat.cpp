#include "packetplayerchat.h"

#include <QStringList>

#include "core/window.h"
#include "game/multiplayergame.h"

extern Window* w;
PacketPlayerChat::PacketPlayerChat(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketPlayerChat::PacketPlayerChat(QString username, QString message) : Packet(PacketType::PLAYER_CHAT)
{
    writePlayerChatData(username, message);
}

PacketPlayerChat::PacketPlayerChat() : Packet(PacketType::PLAYER_CHAT)
{
    valid = false;
}

void PacketPlayerChat::writePlayerChatData(QString username, QString message)
{
    QString data = username + "~" + message;
    this->writeData(data);
}

bool PacketPlayerChat::validate()
{
    valid = false;

    QString data = this->getData();
    QStringList splitData = data.split("~");
    if(splitData.length() != 2)
    {
        errorMsg = "PLAYER_CHAT packet error: The packet format is wrong!";
        return false;
    }

    valid = true;
    return true;
}

bool PacketPlayerChat::execute()
{
    if(!valid) return false;

    w->getMultiplayerGame()->getChatBox()->addMessage(getUsername(),
                                                      getMessage());

    return true;
}

QString PacketPlayerChat::getUsername()
{
    QString username = "";
    if(!valid) return username;

    QString data = this->getData();
    QStringList splitData = data.split("~");
    username = splitData[0];
    return username;
}

QString PacketPlayerChat::getMessage()
{
    QString message = "";
    if(!valid) return message;

    QString data = this->getData();
    QStringList splitData = data.split("~");
    message = splitData[1];
    return message;
}
