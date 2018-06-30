#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <vector>
#include <QString>

#define CONFIG_FILE_NAME "config.txt"

class Config
{
    std::fstream file;
public:
    Config();

    void write(QString key, QString value); // Used for writing a new value or rewriting old one
    QString read(QString key, bool* found = nullptr); // Used to return a value with a key and finding out if the key exists
};

#endif // CONFIG_H
