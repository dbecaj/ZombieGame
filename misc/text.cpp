#include "text.h"

Text::Text()
{
    Text("");
}

Text::Text(QString text)
{
    Text(text, QColor(0, 0, 0));
}

Text::Text(QString text, QColor color)
{
    Text(text, color, QFont("arial", 12));
}

Text::Text(QString text, QColor color, QFont font)
{
    this->text = text;
    this->color = color;
    this->font = font;
}
