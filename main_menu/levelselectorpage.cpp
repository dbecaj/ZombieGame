#include "levelselectorpage.h"

#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>
#include <QGraphicsSceneWheelEvent>

#include "core/window.h"
#include "mainmenu.h"
#include "game/game.h"
#include "misc/language.h"
#include "core/inputmanager.h"
#include "misc/musicmanager.h"

extern Window* w;
LevelSelectorPage::LevelSelectorPage(QObject *parent) : Scene(SceneType::LEVEL_SELECTOR, parent)
{
    this->setSceneRect(0.0f, 0.0f, w->getWindowWidth(), w->getWindowHeight());
    backButton = new Button(10.0f, 10.0f, 100.0f, 100.0f, QPixmap(":/images/textures/back_button_idle.png"),
                            QPixmap(":/images/textures/back_button_active.png"));
    this->addItem(backButton);
    this->connect(backButton, SIGNAL(clickEvent()), this, SLOT(backButtonClick()));

    QPixmap idleButton = QPixmap(":/images/textures/multiplayer_page_button_idle.png");
    QPixmap activeButton = QPixmap(":/images/textures/multiplayer_page_button_active.png");
    float middleX = w->getWindowWidth()/2.0f - LEVELLISTITEM_WIDTH/2.0f;
    loadButton = new Button(middleX, w->getWindowHeight() - 75.0f, 225.0f, 50.0f, idleButton, activeButton,
                            Text(Language::get("load"), QColor(255, 255, 255), QFont("VCR OSD Mono", 20)));
    loadButton->setButtonEnabled(false);
    this->connect(loadButton, SIGNAL(clickEvent()), this, SLOT(loadButtonClick()));
    this->addItem(loadButton);

    deleteButton = new Button(middleX + 275.0f, w->getWindowHeight() - 75.0f, 225.0f, 50.0f, idleButton, activeButton,
                            Text(Language::get("delete"), QColor(255, 255, 255), QFont("VCR OSD Mono", 20)));
    deleteButton->setButtonEnabled(false);
    this->connect(deleteButton, SIGNAL(clickEvent()), this, SLOT(deleteButtonClick()));
    this->addItem(deleteButton);
}

void LevelSelectorPage::init()
{
    changeButtonsStatus(false);

    float middleX = w->getWindowWidth()/2.0f - LEVELLISTITEM_WIDTH/2.0f;
    float lastY = 50.0f;
    QDir levelsDir(QCoreApplication::applicationDirPath() + "/levels");
    QFileInfoList levelsInfo = levelsDir.entryInfoList();
    for(int i = 0; i < levelsInfo.length(); i++)
    {
        QFileInfo levelInfo = levelsInfo[i];
        QStringList levelName = levelInfo.fileName().split(".");
        if(levelName.last() != "lvl" && levelName.last() != "txt") continue;

        LevelListItem* level = new LevelListItem(middleX, lastY, levelInfo.fileName(), levelInfo.created().date());
        levelList.push_back(level);
        this->addItem(level);
        this->connect(level, SIGNAL(clicked(QString)), this, SLOT(levelClick(QString)));
        lastY += LEVELLISTITEM_HEIGHT;
        level = nullptr;
    }
}

void LevelSelectorPage::cleanUp()
{
    for(int i = 0; i < levelList.length(); i++)
        delete levelList[i];

    levelList.clear();
}

void LevelSelectorPage::levelClick(QString levelName)
{
    w->getGame()->init(QCoreApplication::applicationDirPath() + "/levels/" + levelName);
    w->setCurrentScene(w->getGame());
    MusicManager::stop();
}

void LevelSelectorPage::changeButtonsStatus(bool status)
{
    loadButton->setButtonEnabled(status);
    deleteButton->setButtonEnabled(status);
}

void LevelSelectorPage::backButtonClick()
{
    w->setCurrentScene(w->getMainMenu());
}

void LevelSelectorPage::loadButtonClick()
{
    for(int i = 0; i < levelList.length(); i++)
    {
        if(levelList[i]->isHighlighted())
        {
            levelClick(levelList[i]->getLevelName());
            break;
        }
    }

    changeButtonsStatus(false);
}

void LevelSelectorPage::deleteButtonClick()
{
    for(int i = 0; i < levelList.length(); i++)
    {
        if(levelList[i]->isHighlighted())
        {
            QFile level(QCoreApplication::applicationDirPath() + "/levels/" + levelList.at(i)->getLevelName());
            level.remove();
            removeLevel(i);
        }
    }

    changeButtonsStatus(false);
}

void LevelSelectorPage::removeLevel(int index)
{
    delete levelList[index];
    levelList.removeAt(index);

    if(index == levelList.length()) return;

    for(int i = index; i < levelList.length(); i++)
        levelList.at(i)->setPos(levelList[i]->pos().x(), levelList[i]->pos().y() - LEVELLISTITEM_HEIGHT);
}

void LevelSelectorPage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF loadRect(loadButton->pos().x(), loadButton->pos().y(),
                       loadButton->boundingRect().width(), loadButton->boundingRect().height());
    QRectF deleteRect(deleteButton->pos().x(), deleteButton->pos().y(),
                      deleteButton->boundingRect().width(), deleteButton->boundingRect().height());

    if(!(loadRect.contains(w->getInputManager()->mousePosition) ||
         deleteRect.contains(w->getInputManager()->mousePosition)))
    {
        for(int i = 0; i < levelList.length(); i++)
            levelList[i]->setHighlighted(false);

        loadButton->setButtonEnabled(false);
        deleteButton->setButtonEnabled(false);
    }

    this->update();

    QGraphicsScene::mousePressEvent(event);
}

void LevelSelectorPage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void LevelSelectorPage::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->accept();
}
