#include "levelsizepage.h"

#include <QKeyEvent>

#include "main_menu/inserttext.h"
#include "core/window.h"
#include "main_menu/mainmenu.h"
#include "levelcreator.h"
#include "misc/language.h"
#include "main_menu/button.h"
#include "misc/notification.h"
#include "misc/musicmanager.h"

extern Window* w;
LevelSizePage::LevelSizePage(QObject *parent) : Scene(SceneType::LEVEL_SIZE_PAGE, parent)
{
    insertWidth = nullptr;
    insertHeight = nullptr;
    backButton = nullptr;
    background = nullptr;
}

void LevelSizePage::init()
{
    this->setSceneRect(0.0f, 0.0f, w->getWindowWidth(), w->getWindowHeight());
    QSizeF middle(w->getWindowWidth()/2.0f, w->getWindowHeight()/2.0f);

    backButton = new Button(10.0f, 10.0f, 100.0f, 100.0f, QPixmap(":/images/textures/back_button_idle.png"), QPixmap(":/images/textures/back_button_active.png"));
    this->connect(backButton, SIGNAL(clickEvent()), this, SLOT(backButtonClicked()));
    this->addItem(backButton);

    background = new QGraphicsPixmapItem(QPixmap(":/images/textures/list_frame.png"));
    background->setPos(w->getWindowWidth()/2.0f - background->pixmap().width()/2.0f,
                       w->getWindowHeight()/2.0f - background->pixmap().height()/2.0f + 20.0f);
    this->addItem(background);

    QGraphicsTextItem* nameText = new QGraphicsTextItem();
    nameText->setFont(QFont("VCR OSD Mono", 20));
    nameText->setDefaultTextColor(Qt::black);
    nameText->setPlainText(Language::get("level_name") + "  : ");
    QFontMetrics fM(nameText->font());
    nameText->setPos(middle.width() - fM.width(nameText->toPlainText()), middle.height() - 25);
    this->addItem(nameText);

    QGraphicsTextItem* widthText = new QGraphicsTextItem();
    widthText->setFont(QFont("VCR OSD Mono", 20));
    widthText->setDefaultTextColor(Qt::black);
    widthText->setPlainText(Language::get("level_width") + " : ");
    widthText->setPos(nameText->pos().x(), middle.height());
    this->addItem(widthText);

    QGraphicsTextItem* heightText = new QGraphicsTextItem();
    heightText->setFont(QFont("VCR OSD Mono", 20));
    heightText->setDefaultTextColor(Qt::black);
    heightText->setPlainText(Language::get("level_height") + ": ");
    heightText->setPos(nameText->pos().x(), middle.height() + 25); // to get below the top text
    this->addItem(heightText);

    insertName = new InsertText(nameText->pos().x() + (fM.width(nameText->toPlainText())), nameText->pos().y(), 2, nameText->font(), -1, InputType::ALL);
    this->addItem(insertName);

    insertWidth = new InsertText(widthText->pos().x() + (fM.width(widthText->toPlainText())), widthText->pos().y(), 2, widthText->font(), -1, InputType::NUMBERS_ONLY);
    this->addItem(insertWidth);
    insertWidth->showCursor(false);

    insertHeight = new InsertText(heightText->pos().x() + (fM.width(heightText->toPlainText())) + 1.0f, heightText->pos().y(), 2, heightText->font(), -1, InputType::NUMBERS_ONLY);
    this->addItem(insertHeight);
    insertHeight->showCursor(false);
}

void LevelSizePage::cleanUp()
{   
    for(int i = 0; i < this->items().length(); i++)
        delete this->items()[i];

    insertWidth = nullptr;
    insertHeight = nullptr;
    backButton = nullptr;
}

void LevelSizePage::backButtonClicked()
{
    cleanUp();
    w->setCurrentScene(w->getMainMenu());
}

void LevelSizePage::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if(insertName->getText().length() == 0)
        {
            insertWidth->showCursor(false);
            insertHeight->showCursor(false);
            insertName->showCursor(true);
        }
        else if(insertWidth->getText().length() == 0)
        {
            insertName->showCursor(false);
            insertHeight->showCursor(false);
            insertWidth->showCursor(true);
        }
        else if(insertHeight->getText().length() == 0)
        {
            insertName->showCursor(false);
            insertWidth->showCursor(false);
            insertHeight->showCursor(true);
        }
        else
        {
            if(insertWidth->getText().toInt() < 4 || insertHeight->getText().toInt() < 4)
            {
                Notification::show(Language::get("level_size_error"), NotificationType::ERROR);
                return;
            }
            w->getLevelCreator()->init(insertName->getText(), insertWidth->getText().toInt(), insertHeight->getText().toInt());
            cleanUp();
            w->setCurrentScene(w->getLevelCreator());
            MusicManager::play(MusicType::LEVEL_CREATOR);
        }
    }
    else if(event->key() == Qt::Key_Escape)
    {
        cleanUp();
        w->setCurrentScene(w->getMainMenu());
    }
    else if(event->key() == Qt::Key_Up)
    {
        if(insertHeight->isCursorVisible())
        {
            insertName->showCursor(false);
            insertHeight->showCursor(false);
            insertWidth->showCursor(true);
        }
        else if(insertWidth->isCursorVisible())
        {
            insertHeight->showCursor(false);
            insertWidth->showCursor(false);
            insertName->showCursor(true);
        }
    }
    else if(event->key() == Qt::Key_Down)
    {
        if(insertName->isCursorVisible())
        {
            insertName->showCursor(false);
            insertHeight->showCursor(false);
            insertWidth->showCursor(true);
        }
        else if(insertWidth->isCursorVisible())
        {
            insertName->showCursor(false);
            insertWidth->showCursor(false);
            insertHeight->showCursor(true);
        }
    }
    else
    {
        if(insertName->isCursorVisible()) insertName->keyPressEvent(event);
        else if(insertWidth->isCursorVisible()) insertWidth->keyPressEvent(event);
        else insertHeight->keyPressEvent(event);
    }
}
