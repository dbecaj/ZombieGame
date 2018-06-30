#include "packeterror.h"

PacketError::PacketError(QByteArray data) : Packet(data)
{
    valid  = validate();
}

PacketError::PacketError(QString errorMsg) : Packet(PacketType::ERROR)
{
    writeError(errorMsg);
}

PacketError::PacketError() : Packet(PacketType::ERROR)
{
    valid = false;
}

void PacketError::writeError(QString error)
{
    this->writeData(error);
}

bool PacketError::validate()
{
    valid = true;

    return true;
}

bool PacketError::execute()
{
    if(!valid) return false;

    return true;
}

QString PacketError::getError()
{
    if(!valid) return "";

    return getData();
}
