#include "inserttext.h"

#include <QPen>
#include <QFont>
#include <QKeyEvent>
#include <QDebug>
#include <QFontMetrics>
#include <math.h>

InsertText::InsertText(float x, float y, float cursorWidth, QFont font, int textLimit, InputType type, QGraphicsItem *parent) : QGraphicsItemGroup(parent)
{
    text = "";
    this->type = type;
    this->textLimit = textLimit;
    textDisplayLimit = -1;
    cursorDisplayLimit = -1.0f;
    this->setPos(x, y);

    input = new QGraphicsTextItem(this);
    input->setFont(font);
    input->setPos(0, 0);

    QFontMetrics fM(font);
    int cursorHeight = fM.height();

    cursorLine = new QGraphicsLineItem(0, 8, 0, cursorHeight, this); // the alligment is way off dont know why everything is set to 0,0
    cursorLine->setPos(0, 0);
    cursorLine->setPen(QPen(Qt::black, cursorWidth));
    cursorTimer = new QTimer();
    this->connect(cursorTimer, SIGNAL(timeout()), this, SLOT(blinkCursor()));
    cursorTimer->start(500);
}

InsertText::~InsertText()
{
    cursorTimer->stop();
    delete cursorTimer;
}

void InsertText::blinkCursor()
{
    if(cursorLine->isVisible()) cursorLine->hide();
    else cursorLine->show();
}

void InsertText::showCursor(bool show)
{
    if(show)
    {
        if(!cursorTimer->isActive()) cursorTimer->start();
        cursorLine->show();
    }
    else
    {
        cursorTimer->stop();
        cursorLine->hide();
    }

}

void InsertText::clearText()
{
    text = "";
    input->setPlainText(text);
    QFontMetrics m(input->font());
    int textLenght = m.width(text);
    cursorLine->setPos(textLenght + 5.0f, cursorLine->pos().y());
}

void InsertText::changeTextColor(QColor color)
{
    input->setDefaultTextColor(color);
}

void InsertText::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace)
    {
        if(text.length() == 0) return;

        text = text.mid(0, text.length() - 1);
    }
    else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
    }
    else if(event->key() == Qt::Key_Shift) return;
    else if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Down || event->key() == Qt::Key_Right || event->key() == Qt::Key_Left) return;
    else
    {
        if(textLimit < 0);
        else if(text.length() == textLimit) return;

        if(type == InputType::NUMBERS_ONLY)
        {
            if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
            {
                QChar c(event->key());
                text += c;
            }
            else return;
        }
        else if(type == InputType::IPV4)
        {
            if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9 || event->key() == Qt::Key_Period)
            {
                QChar c(event->key());
                text += c;
            }
            else return;
        }
        else // InputType::ALL
        {
            QChar c(event->key());
            if(event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z)
            {
                Qt::KeyboardModifiers keyMod = event->modifiers();
                bool isUpperCase = keyMod.testFlag(Qt::ShiftModifier);
                if(!isUpperCase) c = c.toLower();
            }
            text += c;
        }
    }

    QString displayText = text;
    if(cursorDisplayLimit > 0.0f)
    {
        QFontMetrics m(input->font());
        int textLength = m.width(displayText);
        while(textLength > cursorDisplayLimit)
        {
            displayText = displayText.mid(1);
            textLength = m.width(displayText);
        }
    }

    input->setPlainText(displayText);
    // Reset the cursor position
    QFontMetrics m(input->font());
    int textLength = m.width(displayText);
    cursorLine->setPos(textLength + 5.0f, cursorLine->pos().y());
}
