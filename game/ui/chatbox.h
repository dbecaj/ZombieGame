#ifndef CHATBOX_H
#define CHATBOX_H

#include <QGraphicsRectItem>

#include "main_menu/inserttext.h"

class ChatBox : public QGraphicsRectItem
{
    QList<QGraphicsTextItem*> textList;
    InsertText* insertText;
    QRectF textBox;
    QRectF insertBox;
    int visableTopIndex;
    int visableBottomIndex;

    int reserveLines(QString text);
    void hideLines();
    void resetLines();
public:
    ChatBox(float x, float y, float width, float height, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void addMessage();
    void addMessage(QString message);
    void addMessage(QString username, QString message);
    void addText(QString text);
    void addText(QString text, QColor color);
    void scrollUp();
    void scrollDown();

    InsertText* getInsertText() { return insertText; }
};

#endif // CHATBOX_H
