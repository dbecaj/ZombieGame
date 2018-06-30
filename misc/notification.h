#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>
#include <QTimer>

#include "notificationbar.h"

#define RECT_HEIGHT 60.0f

enum class NotificationType
{
    NORMAL,
    ALERT,
    ERROR
};

class Notification : public QObject
{
    Q_OBJECT

    static QTimer timer;
    static Notification notif;
    static NotificationBar* notifBar;
    static bool down;
    static int moveAmmount;
private slots:
    void move();
    void notifBarDestroyed(QObject* object);
public:
    explicit Notification(QObject* parent = nullptr);

    static void show(QString text, NotificationType type = NotificationType::NORMAL);
};

#endif // NOTIFICATION_H
