#include "config.h"

#include <QString>
#include <QList>
#include <QCoreApplication>

Config::Config()
{
    file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::in);
    if(!file.is_open())
    {
        file.close();
        file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::out); // if the config doesn't exist create one
    }

    file.close();
}

void Config::write(QString key, QString value)
{
    bool found;
    QString tmp = read(key, &found);
    if(found) // if the key exists in the config then we just change it
    {
        file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::in);

        // Temporary save the config.txt data to a list and change the necessary line (we can do this because there isn't much data to save)
        QList<std::string> configData;
        std::string line;
        while(std::getline(file, line))
        {
            QStringList keyAndValue = QString::fromStdString(line).split("=");
            if(keyAndValue[0] == key) // we found the value that needs to be changed
            {
                line = QString(key + "=" + value).toStdString();
            }
            configData.push_back(line);
        }

        file.close();
        // Then we just rewrite the config.txt with the modified line
        file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::out);

        for(int i = 0; i < configData.length(); i++)
            file << configData[i];

        file.close();

        return;
    }

    // If the key does not exist we just write it at the end
    file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::app);

    file << key.toStdString() << "=" << value.toStdString() << "\n"; // the stream only knows how to write std::string

    file.close();
}

QString Config::read(QString key, bool *found)
{
    file.open(QCoreApplication::applicationDirPath().toStdString() + "/" + CONFIG_FILE_NAME, std::ios::in);

    // We loop through the config.txt data and if we find the key we set the value of the found pointer to true and return the value
    std::string line;
    while(std::getline(file, line))
    {
        QStringList keyAndValue = QString::fromStdString(line).split("=");
        if(keyAndValue[0] == key)
        {
            file.close();
            if(found) *found = true;
            return keyAndValue[1];
        }
    }

    file.close();

    // If we don't find it we set the found pointer to false and return an empty QString
    if(found) *found = false;
    return "";
}
