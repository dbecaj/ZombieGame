#include "notification.h"

#include <QFontMetrics>
#include <QDebug>
#include <QObject>

#include "core/window.h"
#include "core/scene.h"

extern Window* w;
QTimer Notification::timer;
Notification Notification::notif;
NotificationBar* Notification::notifBar = new NotificationBar(&notif);
bool Notification::down = true;
int Notification::moveAmmount = 0;

Notification::Notification(QObject* parent) : QObject(parent)
{
}

void Notification::show(QString text, NotificationType type)
{
    static bool firstTime = true;
    if(timer.isActive())
    {
        timer.stop();
        notifBar->scene()->removeItem(notifBar);
        notif.disconnect(&timer, SIGNAL(timeout()), &notif, SLOT(move()));
    }

    if(firstTime)
    {
        notif.connect(notifBar, SIGNAL(destroyed(QObject*)), &notif, SLOT(notifBarDestroyed(QObject*)));
        firstTime = false;
    }

    QPointF sceneZero = w->mapToScene(QPoint(0, 0));
    w->getCurrentScene()->addItem(notifBar);
    notifBar->setPosition(sceneZero.x(), sceneZero.y() - NotificationBar::HEIGHT);
    if(type == NotificationType::NORMAL) notifBar->setColor(QColor(255, 255, 255, 240));
    else if(type == NotificationType::ALERT) notifBar->setColor(QColor(255, 255, 0, 240));
    else notifBar->setColor(QColor(170, 0, 0, 240));

    QFont font("VCR OSD Mono", 16);
    font.setBold(true);
    notifBar->setText(Text(text, QColor(0, 0, 0), font));

    moveAmmount = 0;
    down = true;
    timer.setSingleShot(false);
    notif.connect(&timer, SIGNAL(timeout()), &notif, SLOT(move()));
    timer.start(5);

    notifBar->setZValue(5.0f);
}

void Notification::move()
{
    moveAmmount++;
    QPointF sceneZero = w->mapToScene(QPoint(0,0));
    if(down)
    {
        notifBar->setY(sceneZero.y() - NotificationBar::HEIGHT + moveAmmount);
        if(notifBar->y() >= w->mapToScene(0.0f, 0.0f).y())
        {
            down = false;
            moveAmmount = 0;
            timer.setInterval(3000);
        }
    }
    else
    {
        timer.setInterval(5);
        notifBar->setY(sceneZero.y() - moveAmmount);
        if(notifBar->y() <= (sceneZero.y() - NotificationBar::HEIGHT))
        {
            down = true;
            timer.stop();
            notif.disconnect(&timer, SIGNAL(timeout()), &notif, SLOT(move()));
            if(notifBar->scene() != 0) notifBar->scene()->removeItem(notifBar);
        }
    }
}

void Notification::notifBarDestroyed(QObject *object)
{
    notifBar = new NotificationBar(&notif);
    timer.stop();
    notif.disconnect(&timer, SIGNAL(timeout()), &notif, SLOT(move()));
}
