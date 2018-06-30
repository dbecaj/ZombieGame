#include "language.h"

#include <QFile>
#include <QDomDocument>

Language::Language()
{
}

bool Language::initialized = false;
QString Language::errorMsg = "";
QMap<QString, QString> Language::strings;

bool Language::init(QString langXmlPath)
{
    initialized = false;
    if(langXmlPath.split(".").last() != "xml")
    {
        errorMsg = "ERROR: The file " + langXmlPath + " is not xml format!";
        return false;
    }

    QFile file(langXmlPath);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        errorMsg = "ERROR: Could not open the xml file: " + langXmlPath;
        return false;
    }

    QDomDocument doc;
    if(!doc.setContent(&file, false, &errorMsg)) return false;

    QDomElement root = doc.documentElement();
    QDomNode node;
    for(int i = 0; i < root.childNodes().length(); i++)
    {
        node = root.childNodes().at(i);
        if(node.toElement().tagName() != "string")
        {
            errorMsg = "ERROR: Element in the lang file has a tag that is not 'string'!";
            return false;
        }
        else if(!node.toElement().hasAttribute("name"))
        {
            errorMsg = "ERROR: Element in the lang file does not have the attribute 'name'!";
            return false;
        }

        strings.insert(node.toElement().attribute("name"), node.toElement().text());
    }

    initialized = true;
    return true;
}

QString Language::get(QString key)
{
    if(!initialized) return "!";

    return strings.value(key);
}
