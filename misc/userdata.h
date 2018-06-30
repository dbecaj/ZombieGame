#ifndef USERDATA_H
#define USERDATA_H

#include <QString>
#include <QList>

class UserData
{
    QList<QString> servers;
public:
    UserData(QString fileName);

    QList<QString> getServerData() { return servers; }

    static void save(QString fileName);
};

#endif // USERDATA_H
