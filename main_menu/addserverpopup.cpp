#include "addserverpopup.h"

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

#include "multiplayerpage.h"
#include "core/window.h"
#include "inserttext.h"
#include "button.h"
#include "core/inputmanager.h"
#include "serverlistitem.h"
#include "multiplayerpage.h"
#include "mainmenu.h"
#include "serverlistitem.h"
#include "misc/language.h"

extern Window* w;
AddServerPopup::AddServerPopup(MultiplayerPage* page, QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{
    // We get MultiplayerPage handle
    mP = page;
    this->setPos(w->getWindowWidth()/2.0f - 500.0f/2.0f, 100.0f);
    this->setPixmap(QPixmap(":/images/textures/add_server_background.png").scaled(500, 300));

    QPixmap idleButton = QPixmap(":/images/textures/pause_button_idle.png");
    QPixmap activeButton = QPixmap(":/images/textures/pause_button_active.png");

    // Add buttons
    addButton = new Button(45.0f, 175.0f, 175.0f, 50.0f, idleButton, activeButton, Text(Language::get("add"), QColor(255, 255, 255), QFont("VCR OSD Mono", 24)), this);
    this->connect(addButton, SIGNAL(clickEvent()), this, SLOT(clickAddButton()));

    cancelButton = new Button(270.0f, 175.0f, 175.0f, 50.0f, idleButton, activeButton, Text(Language::get("cancel"), QColor(255, 255, 255), QFont("VCR OSD Mono", 24)), this);
    this->connect(cancelButton, SIGNAL(clickEvent()), this, SLOT(clickCancelButton()));

    // Init our custom insert text
    insertIP = new InsertText(70.0f, 60.0f, 3.0f, QFont("VCR OSD Mono", 30), 15, InputType::IPV4, this);

    // Set errorMsgText text properties
    errorMsgText = new QGraphicsTextItem(this);
    errorMsgText->setPos(40.0f, 120.0f);
    errorMsgText->setFont(QFont("times", 14));
    errorMsgText->setDefaultTextColor(QColor(155, 0, 0));

    this->setHandlesChildEvents(false);
}

void AddServerPopup::clickAddButton()
{
    // If the IP isn't valid don't add the server and write out the error
    QString errorMsg;
    if(!isValidIP(insertIP->getText(), errorMsg))
    {
        errorMsgText->setPlainText(errorMsg);
        return;
    }

    for(int i = 0; i < w->getMainMenu()->getMultiplayerPage()->getServerList()->length(); i++)
    {
        ServerListItem* item = w->getMainMenu()->getMultiplayerPage()->getServerList()->at(i);
        if(item->getServerIP() == insertIP->getText())
        {
            errorMsgText->setPlainText("This server already exists!");
            return;
        }
    }

    mP->addServer(insertIP->getText());

    // Close the addServerPopup
    mP->removeItem(this);
    mP->setServerPopupOpen(false);
}

bool AddServerPopup::isValidIP(QString address, QString& errorMsg)
{
    // Format check
    QStringList list = address.split(".");
    if(list.length() != 4)
    {
        errorMsg = "Incorrect format should be ###.###.###.###";
        return false;
    }

    if(list[0] == "" || list[1] == "" || list[2] == "" || list[3] == "")
    {
        errorMsg = "Places in the IP address are empty!";
        return false;
    }

    // IP value check
    bool invalidIP = false;
    if(list[0].toInt() >= 255 || list[0].toInt() == 0) invalidIP = true;
    if(list[1].toInt() >= 255) invalidIP = true;
    if(list[2].toInt() >= 255) invalidIP = true;
    if(list[3].toInt() >= 255 || list[3].toInt() == 0) invalidIP = true;
    if(invalidIP)
    {
        errorMsg = "Invalid IP!";
        return false;
    }

    return true;
}

void AddServerPopup::clickCancelButton()
{
    // Close the addServerPopup
    mP->removeItem(this);
    mP->setServerPopupOpen(false);
}

void AddServerPopup::clearInsertBox()
{
    insertIP->clearText();
}

void AddServerPopup::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        mP->removeItem(this);
        mP->setServerPopupOpen(false);
    }
    insertIP->keyPressEvent(event);
    QGraphicsPixmapItem::keyPressEvent(event);
    event->accept();
}
