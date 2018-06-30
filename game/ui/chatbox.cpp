#include "chatbox.h"

#include <QPainter>

#include "core/window.h"

extern Window* w;
ChatBox::ChatBox(float x, float y, float width, float height, QGraphicsItem* parent) : QGraphicsRectItem(parent)
{
    this->setRect(QRectF(x, y, width, height));
    textBox.setRect(x, y, width, height - 30.0f);
    insertText = new InsertText(x + 4.0f, y + height - 27.0f, 2.0f, QFont("times", 14), -1, InputType::ALL, this);
    insertBox.setRect(x, y + (height - 25.0f), width, 25.0f);
    visableTopIndex = 0;
    visableBottomIndex = 0;
}

void ChatBox::addMessage()
{
    addMessage(insertText->getText());
}

void ChatBox::addMessage(QString message)
{
    addMessage(w->getUsername(), message);
}

void ChatBox::addMessage(QString username, QString message)
{
    if(message == "" || username == "") return;
    QString outputText = username + ": " + message;

    addText(outputText);
}

void ChatBox::addText(QString text)
{
    addText(text, QColor(255, 255, 255));
}

void ChatBox::addText(QString text, QColor color)
{
    if(text == "") return;
    resetLines();
    int lines = reserveLines(text);
    QFontMetrics fM(QFont("times", 14));

    int start = 0;
    for(int i = 0; i < lines; i++)
    {
        QString add = "";
        while(fM.width(add) < 280 && start < text.length())
            add += text[start++];

        QGraphicsTextItem* newText = new QGraphicsTextItem(this);
        newText->setFont(QFont("times", 14));
        newText->setPlainText(add);
        newText->setDefaultTextColor(color);
        newText->setPos(4.0f, this->rect().height() + (100.0f - (20.0f * (lines - i))));
        textList.push_back(newText);
    }

    visableBottomIndex = textList.length() - 1;
    if(textList.length() <= 18) visableTopIndex = 0;
    else visableTopIndex = visableBottomIndex - 17;

    hideLines();
}

int ChatBox::reserveLines(QString text)
{
    QFontMetrics fM(QFont("times", 14));
    // We reserve/move 1 lines or multipule lines to make space for new text
    int lines = fM.width(text) / 280;
    if(fM.width(text) % 280 != 0) lines++;
    for(int i = 0; i < textList.length(); i++)
    {
        textList[i]->setPos(4.0f, textList[i]->y() - (20.0f * lines));
    }

    return lines;
}

void ChatBox::resetLines()
{
    int line = 1;
    for(int i = textList.length() - 1; i >= 0; i--)
    {
        float y = (this->rect().height() + 100) - 20.0f * line;
        textList[i]->setPos(4.0f, y);
        line++;
    }
}

void ChatBox::hideLines()
{
    for(QGraphicsTextItem* item : textList)
        item->hide();

    for(int i = visableTopIndex; i <= visableBottomIndex; i++)
        textList[i]->show();
}

void ChatBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(QColor(0, 0, 0)));
    painter->setOpacity(0.3f);
    painter->drawRect(textBox);
    painter->drawRect(insertBox);
}

void ChatBox::scrollUp()
{
    if(visableTopIndex == 0) return;
    for(QGraphicsItem* item : textList)
        item->setPos(4.0f, item->y() + 20.0f);

    visableTopIndex--;
    visableBottomIndex--;

    hideLines();
 }

void ChatBox::scrollDown()
{
    if(visableBottomIndex == textList.length() - 1 || textList.length() == 0) return;
    for(QGraphicsItem* item : textList)
        item->setPos(4.0f, item->y() - 20.0f);

    visableTopIndex++;
    visableBottomIndex++;

    hideLines();
}
