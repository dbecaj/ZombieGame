#include "multiplayerpage.h"

#include <QGraphicsSceneWheelEvent>
#include <QDebug>
#include <QKeyEvent>

#include "serverlistitem.h"
#include "core/window.h"
#include "button.h"
#include "mainmenu.h"
#include "addserverpopup.h"
#include "core/inputmanager.h"
#include "borderlessrect.h"
#include "misc/language.h"

extern Window* w;
MultiplayerPage::MultiplayerPage(QObject *parent) : Scene(SceneType::MULTIPLAYER_PAGE, parent)
{
    serverPopupOpen = false;

    addServerPopup = nullptr;
    backButton = nullptr;
    addServerButton = nullptr;
    refreshButton = nullptr;
    connectButton = nullptr;
    deleteButton = nullptr;
}

void MultiplayerPage::init()
{
    if(addServerPopup) // the page has already been initialized so we just reset the content
    {
        resetContent();
        return;
    }

    addServerPopup = new AddServerPopup(this);
    float middleX = w->getWindowWidth()/2.0f - SERVERLISTITEM_WIDTH/2.0f;

    QPixmap idleButton = QPixmap(":/images/textures/multiplayer_page_button_idle.png");
    QPixmap activeButton = QPixmap(":/images/textures/multiplayer_page_button_active.png");

    // Create all buttons
    backButton = new Button(10.0f, 10.0f, 100.0f, 100.0f, QPixmap(":/images/textures/back_button_idle.png"), QPixmap(":/images/textures/back_button_active.png"));
    this->connect(backButton, SIGNAL(clickEvent()), this, SLOT(clickBackButton()));
    this->addItem(backButton);
    backButton->setZValue(0.01f);

    addServerButton = new Button(middleX, 25.0f, 225.0f, 50.0f, idleButton, activeButton, Text(Language::get("add_server"), QColor(255, 255, 255), QFont("VCR OSD Mono", 20)));
    this->connect(addServerButton, SIGNAL(clickEvent()), this, SLOT(clickAddServerButton()));
    this->addItem(addServerButton);
    addServerButton->setZValue(0.01f);

    refreshButton = new Button(middleX + 275.0f, 25.0f, 225.0f, 50.0f, idleButton, activeButton, Text(Language::get("refresh"), QColor(255, 255, 255), QFont("VCR OSD Mono", 20)));
    this->connect(refreshButton, SIGNAL(clickEvent()), this, SLOT(clickRefreshButton()));
    this->addItem(refreshButton);
    refreshButton->setZValue(0.01f);

    connectButton = new Button(middleX, w->getWindowHeight() - 75.0f, 225.0f, 50.0f, idleButton, activeButton,
                               Text(Language::get("connect"), QColor(255, 255, 255), QFont("VCR OSD Mono", 20)));
    this->connect(connectButton, SIGNAL(clickEvent()), this, SLOT(clickConnectButton()));
    connectButton->setButtonEnabled(false);
    this->addItem(connectButton);
    connectButton->setZValue(0.01f);

    deleteButton = new Button(middleX + 275.0f, w->getWindowHeight() - 75.0f, 225.0f, 50.0f, idleButton, activeButton,
                               Text(Language::get("delete"), QColor(255, 255, 255), QFont("VCR OSD Mono", 20)));
    this->connect(deleteButton, SIGNAL(clickEvent()), this, SLOT(clickDeleteButton()));
    deleteButton->setButtonEnabled(false);
    this->addItem(deleteButton);
    deleteButton->setZValue(0.01f);

    this->setSceneRect(0.0f, 0.0f, w->getWindowWidth(), w->getWindowHeight());
}

MultiplayerPage::~MultiplayerPage()
{
}

void MultiplayerPage::cleanUp()
{
    stopTimers();
}

void MultiplayerPage::resetContent()
{
    for(int i = 0; i < serverList.length(); i++)
        serverList[i]->resetContent();
}

void MultiplayerPage::stopTimers()
{
    for(int i = 0; i < serverList.length(); i++)
        serverList[i]->stopTimers();
}

void MultiplayerPage::addServer(QString ip)
{
    float middleX = w->getWindowWidth()/2.0f - SERVERLISTITEM_WIDTH/2.0f;
    float y;
    if(serverList.length() != 0) y = serverList.last()->y();
    else y = SERVERLISTITEM_HEIGHT - 200.0f;
    ServerListItem* item = new ServerListItem(middleX, y + SERVERLISTITEM_HEIGHT, ip, serverList.length());
    serverList.push_back(item);
    this->addItem(item);
}

void MultiplayerPage::removeServer(int index)
{
    serverList[index]->stopTimers();
    delete serverList[index];
    serverList.removeAt(index);

    if(index == serverList.length()) return;

    for(int i = index; i < serverList.length(); i++)
        serverList[i]->setPos(serverList[i]->pos().x(), serverList[i]->pos().y() - SERVERLISTITEM_HEIGHT);
}

void MultiplayerPage::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->accept();
    if(serverList.length() == 0) return;
    if(event->delta() < 0)
    {
        if(serverList.last()->pos().y() <= 500.0f) return;
    }
    else
    {
        if(serverList.first()->pos().y() == 100.0f) return; // 100.0f is the starting position of the first element
    }

    float moveAmmount = 10.0f;
    if(event->delta() < 0) moveAmmount *= -1.0f;
    for(int i = 0; i < serverList.length(); i++)
    {
        serverList[i]->setPos(serverList[i]->x(), serverList[i]->y() + moveAmmount);
    }
}

void MultiplayerPage::clickBackButton()
{
    stopTimers();

    w->setCurrentScene(w->getMainMenu());
}

void MultiplayerPage::clickAddServerButton()
{
    addServerPopup->clearInsertBox();
    if(!this->items().contains(addServerPopup)) this->addItem(addServerPopup);
    serverPopupOpen = true;
}

void MultiplayerPage::clickRefreshButton()
{
    resetContent();
}

void MultiplayerPage::clickConnectButton()
{
    for(int i = 0; i < serverList.length(); i++)
    {
        if(serverList[i]->isHighlighted())
        {
            serverList[i]->mouseDoubleClickEvent(nullptr);
            break;
        }
    }
}

void MultiplayerPage::clickDeleteButton()
{
    int i = 0;
    for(i = 0; i < serverList.length(); i++)
    {
        if(serverList[i]->isHighlighted())
        {
            removeServer(i);
            connectButton->setButtonEnabled(false);
            deleteButton->setButtonEnabled(false);
            this->update();
            break;
        }
    }
}

void MultiplayerPage::keyPressEvent(QKeyEvent *event)
{
    // If the addServerPopup is open just pass the event to it
    if(serverPopupOpen) addServerPopup->keyPressEvent(event);
    else if(event->key() == Qt::Key_Escape)
    {
        w->setScene(w->getMainMenu());
    }
}

void MultiplayerPage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF connectRect(connectButton->pos().x(), connectButton->pos().y(),
                       connectButton->boundingRect().width(), connectButton->boundingRect().height());
    QRectF deleteRect(deleteButton->pos().x(), deleteButton->pos().y(),
                      deleteButton->boundingRect().width(), deleteButton->boundingRect().height());

    if(!(connectRect.contains(w->getInputManager()->mousePosition) ||
         deleteRect.contains(w->getInputManager()->mousePosition)))
    {
        for(int i = 0; i < serverList.length(); i++)
            serverList[i]->setHighlighted(false);

        connectButton->setButtonEnabled(false);
        deleteButton->setButtonEnabled(false);
    }

    QGraphicsScene::mousePressEvent(event);
}

void MultiplayerPage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(serverPopupOpen) return;
    QGraphicsScene::mouseDoubleClickEvent(event);
}
