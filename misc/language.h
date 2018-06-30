#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QMap>

class Language
{
    static QMap<QString, QString> strings;
    static bool initialized;
    static QString errorMsg;
public:
    Language();

    static bool init(QString langXmlPath);

    static QString get(QString key);
    static bool isInitialized() { return initialized; }
    static QString getErrorMsg() { return errorMsg; }
};

#endif // LANGUAGE_H
