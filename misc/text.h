#ifndef TEXT_H
#define TEXT_H


#include <QString>
#include <QColor>
#include <QFont>

class Text
{
    QString text;
    QColor color;
    QFont font;
public:
    Text();
    Text(QString text);
    Text(QString text, QColor color);
    Text(QString text, QColor color, QFont font);

    QString getText() { return text; }
    QColor getColor() { return color; }
    QFont getFont() { return font; }

    void setText(QString text) { this->text = text; }
    void setColor(QColor color) { this->color = color; }
    void setFont(QFont font) { this->font = font; }
};

#endif // TEXT_H
