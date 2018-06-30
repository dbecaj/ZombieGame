#include "userdata.h"

#include <fstream>
#include <QCoreApplication>
#include <string>
#include <QDebug>
#include <iostream>

#include "core/window.h"
#include "main_menu/mainmenu.h"
#include "main_menu/multiplayerpage.h"
#include "main_menu/serverlistitem.h"

extern Window* w;
UserData::UserData(QString fileName)
{
    std::fstream file;
    std::string endPath = QCoreApplication::applicationDirPath().toStdString() + "/" + fileName.toStdString();
    file.open(endPath, std::ios::in | std::ios::binary);
    if(!file.is_open())
    {
        file.close();
        return;
    }

    int listSize = 0;
    int lineSize = 0;

    file.read((char*)&listSize, sizeof(int));
    servers.reserve(listSize);
    while(file.read((char*)&lineSize, sizeof(int)))
    {
        std::string ip;
        ip.resize(lineSize);
        file.read((char*)&ip[0], lineSize);
        servers.push_back(QString::fromStdString(ip));
    }

    file.close();
}

void UserData::save(QString fileName)
{
    std::fstream file;
    std::string endPath = QCoreApplication::applicationDirPath().toStdString() + "/" + fileName.toStdString();
    file.open(endPath, std::ios::out | std::ios::binary);

    // [listSize][lineLength][ip][lineLength][ip]...
    QList<ServerListItem*>* serverList = w->getMainMenu()->getMultiplayerPage()->getServerList();
    int serverListSize = serverList->length();
    file.write((char*)&serverListSize, sizeof(int));
    for(int i = 0; i < serverList->length(); i++)
    {
        ServerListItem* item = serverList->at(i);
        QString ip = item->getServerIP();
        int ipLength = ip.length();
        file.write((char*)&ipLength, sizeof(int));
        file.write(ip.toStdString().c_str(), ipLength);
    }
    file.close();
}
