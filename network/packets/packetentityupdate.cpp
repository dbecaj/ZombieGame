#include "packetentityupdate.h"

#include <QPointF>

#include "core/window.h"
#include "game/multiplayergame.h"
#include "network/entities/zombiemp.h"
#include "network/entities/scientistmp.h"

extern Window* w;
PacketEntityUpdate::PacketEntityUpdate(QByteArray data) : Packet(data)
{
    valid = validate();
}

PacketEntityUpdate::PacketEntityUpdate(QList<ZombieMP *> zombies, QList<ScientistMP *> scientists) : Packet(PacketType::ENTITY_UPDATE)
{
    writeEntityUpdate(zombies, scientists);
}

PacketEntityUpdate::PacketEntityUpdate() : Packet(PacketType::ENTITY_UPDATE)
{
    valid = false;
}

void PacketEntityUpdate::writeEntityUpdate(QList<ZombieMP *> zombies, QList<ScientistMP *> scientists)
{
    QString zombiesData = ""; // [zombieId]/[x]/[y]/[lives]*
    for(int i = 0; i < w->getMultiplayerGame()->getZombies().length(); i++)
    {
        ZombieMP* z = w->getMultiplayerGame()->getZombies()[i];
        zombiesData += QString::number(z->getId()) + "/" + QString::number(z->pos().x()) + "/" +
                       QString::number(z->pos().y() + (MOB_HEIGHT - MOB_COLL_HEIGHT)) + "/" + QString::number(z->getLives()) + "*";
    }
    if(zombiesData.length() != 0) zombiesData = zombiesData.mid(0, zombiesData.length() - 1);

    QString scientistsData = ""; // [scientistId]/[x]/[y]/[dir]*
    for(int i = 0; i < w->getMultiplayerGame()->getScientists().length(); i++)
    {
        Scientist* s = w->getMultiplayerGame()->getScientists()[i];
        scientistsData += QString::number(s->getId()) + "/" + QString::number(s->pos().x()) + "/" +
                          QString::number(s->pos().y() + (MOB_HEIGHT - MOB_COLL_HEIGHT)) + "/" + QString::number(s->getDirection()) + "*";
    }
    if(scientistsData.length() != 0) scientistsData = scientistsData.mid(0, scientistsData.length() - 1);

    QString data = zombiesData + "," + scientistsData;
    this->writeData(data);
}

bool PacketEntityUpdate::validate()
{
    valid = false;

    QString data = getData();
    QStringList splitData = data.split(',');
    if(splitData.length() != 2)
    {
        errorMsg = "ENTITY_UPDATE packet error: The packet format is the wrong format!";
        return false;
    }

    if(splitData[0].split('*').length() != 0 && splitData[0] != "")
    {
        if(splitData[0].split('*')[0].split('/').length() != 4)
        {
            errorMsg = "ENTITY_UPDATE packet error: The packet format for zombies is the wrong format!";
            return false;
        }
    }

    if(splitData[1].split('*').length() != 0 && splitData[1] != "")
    {
        if(splitData[1].split('*')[0].split('/').length() != 4)
        {
            errorMsg = "ENTITY_UPDATE packet error: The packet format for scientists is the wrong format!";
            return false;
        }
    }

    valid = true;
    return true;
}

bool PacketEntityUpdate::execute()
{
    if(!valid) return false;

    QString data = getData();
    QStringList splitData = data.split(',');
    for(QString zombieData : splitData[0].split('*'))
    {
        if(zombieData == "") continue;
        QStringList splitZombieData = zombieData.split('/');
        bool ok = false;
        long id = splitZombieData[0].toInt(&ok);
        float x = splitZombieData[1].toFloat(&ok);
        float y = splitZombieData[2].toFloat(&ok);
        int lives = splitZombieData[3].toInt(&ok);
        if(!ok)
        {
            qDebug() << "ENTITY_UPDATE zombie: the conversion failed!";
            continue;
        }

        ZombieMP* zombie = w->getMultiplayerGame()->getZombie(id);
        if(!zombie)
        {
            qDebug() << "ENTITY_UPDATE zombie: " << id << " does not exist!";
            continue;
        }

        zombie->setPos(x, y);
        zombie->setLives(lives);
    }

    for(QString scientistData : splitData[1].split('*'))
    {
        if(scientistData == "") continue;
        QStringList splitScientistData = scientistData.split('/');
        bool ok = false;
        long id = splitScientistData[0].toInt(&ok);
        float x = splitScientistData[1].toFloat(&ok);
        float y = splitScientistData[2].toFloat(&ok);
        float dir = splitScientistData[3].toFloat(&ok);
        if(!ok)
        {
            qDebug() << "ENTITY_UPDATE scientist the conversion failed!";
            continue;
        }

        ScientistMP* scientist = w->getMultiplayerGame()->getScientist(id);
        if(!scientist)
        {
            qDebug() << "ENTITY_UPDATE scientist " << id << " does not exist!";
            continue;
        }

        scientist->setPos(x, y);
        scientist->changeAngle(dir);
    }

    return true;
}
