#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>

enum PacketType
{
    INVALID = -1,
    BASIC = 0, // used for messages from server/client very basic get it :3 / FORMAT: [message] TCP/UDP
    PING = 1, // used to measure ping (packet has no data) UDP
    INFO = 2, // used to to get server name / FORMAT: [serverName],[numberOfConnectedPlayers],[numberOfMaxPlayers] TCP
    JOIN = 3, // used to join the server (for you to join the server has to send the same packet back) / FORMAT: [playerName] TCP
    DISCONNECT = 4, // is send by the client when he disconnects / FORMAT: [playerName] TCP
    ERROR = 5, // used to send varius error messages / FORMAT: [errorMessage] TCP/UDP
    SERVER_SHUTDOWN = 6, // used to signal clients that the server has stoped with the 'stop' command TCP
    LEVEL_DATA = 7, // used to send level layout / FORMAT: [levelLine]#[levelLine2]... TCP
    ENTITY_DATA = 8, // used to send information about the number of entites / FORMAT: [playerName]/[x]/[y]*...,[zombieId]/[x]/[y]/[lives]*...,[scientistId]/[gender]/[x]/[y]/[dir]*... TCP
    PLAYER_POS = 9, // used to send player position / FORMAT: [playerName],[x],[y],[degrees] UDP
    ZOMBIE_CHANGE_TARGET = 10, // used to send when zombie changes target / FORMAT: [id],[x],[y],[mobType],[idTarget/playerName] TCP
    SCIENTIST_CHANGE_DIR = 11, // used to send when scientist changes direction / FORMAT: [id],[x],[y][newDir] UDP
    KICK = 12, // used to message to player that they have been kicked / FORMAT: [playerName] TCP
    FIRE_BULLET = 13, // send when players fires a bullet / FORMAT: [playerName],[x],[y],[direction] UDP
    ZOMBIE_HIT = 14, // send when zombie hits a mob that isn't a zombie / FORMAT: [idZombie],[zombieX],[zombieY],[targetX],[targetY],[mobType],[idTarget/playerName],[newZombieId] TCP
    BULLET_HIT = 15, // sends data about the hit zombie when the zombie is hit / FORMAT: [idZombie]/[lives],[playerName] TCP
    PLAYER_CHAT = 16, // sends message that the player typed in the chatbox / FORMAT: [playerName]~[message] TCP
    INFECTED = 17, // used when a zombie touches a player and the player is infected / FORMAT: [playerName],[newZombieId] TCP
    ENTITY_UPDATE = 18 // used to update the entities so they stay in sync with the game / FORMAT: [zombieId]/[x]/[y]/[y]*,[scientistId]/[x]/[y][dir]*... UDP
};

class Packet
{
protected:
    QByteArray data;
    PacketType type;
    QString errorMsg;
    bool valid;
    quint32 packetSize;

    void writeData(QString data);
public:
    Packet(QByteArray data);
    Packet(PacketType type);

    virtual bool execute() = 0;
    virtual bool validate() = 0;

    PacketType getPacketType();
    static PacketType determinePacketType(QByteArray data);
    static QString extractData(QByteArray data);
    QString getErrorMsg() { return errorMsg; }
    bool isValid() { return valid; }
    QByteArray getData(bool full = false); // Returns the packets data(if full = true it will also return the header
    quint32 getSize() { return packetSize; }
    static quint32 determinePacketSize(QByteArray data);
    static QString translatePacket(QByteArray data);
};

#endif // PACKET_H
