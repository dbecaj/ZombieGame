#include "inputpage.h"

#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QKeyEvent>
#include <QPointF>
#include <QFontMetrics>

#include "core/window.h"
#include "mainmenu.h"
#include "inserttext.h"
#include "misc/language.h"

extern Window* w;
InputPage::InputPage(QObject *parent) : Scene(SceneType::INPUT_PAGE, parent)
{
    insertText = nullptr;
}

void InputPage::init()
{
    this->setSceneRect(0, 0, w->getWindowWidth(), w->getWindowHeight());

    // Output the "Insert player name:" text
    QGraphicsTextItem* inputText = new QGraphicsTextItem();
    inputText->setDefaultTextColor(Qt::black);
    inputText->setFont(QFont("times", 30));
    inputText->setPlainText("Insert player name:");
    inputText->setPos(100.0f, 300.0f);
    this->addItem(inputText);

    QFontMetrics fM(inputText->font());

    // Create out custom InputText manager and place it's cursor at the end of the output text
    insertText = new InsertText(fM.width(Language::get("insert_player_name") + ": ") + inputText->pos().x(), inputText->pos().y(), 3, QFont("times", 30), 20);
    this->addItem(insertText);
}

void InputPage::cleanUp()
{
    for(QGraphicsItem* item : this->items())
        delete item;
}

void InputPage::keyPressEvent(QKeyEvent *event)
{
    // If we press Enter the inputed value gets writen in the config and the scene gets changed to mainMenu
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        w->getConfig()->write("PlayerName", insertText->getText());
        w->setCurrentScene(w->getMainMenu());
        return;
    }

    // Pass the event to InsertText because the QT is just weird ps: yes I did try QGraphicsScene::keyPressEvent(event); it didn't work wise guy
    insertText->keyPressEvent(event);
}
