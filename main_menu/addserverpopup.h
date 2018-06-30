#ifndef ADDSERVERPOPUP_H
#define ADDSERVERPOPUP_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QObject>

class MultiplayerPage;
class InsertText;
class Button;

class AddServerPopup : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    MultiplayerPage* mP;
    InsertText* insertIP;
    Button* addButton;
    Button* cancelButton;
    QGraphicsTextItem* errorMsgText; // This only displays something if something goes wrong

    bool isValidIP(QString address, QString& errorMsg);
private slots:
    void clickAddButton();
    void clickCancelButton();
public:
    explicit AddServerPopup(MultiplayerPage *page, QGraphicsItem *parent = nullptr);

    void clearInsertBox(); // Clears insertIP of text

    void keyPressEvent(QKeyEvent *event);
};

#endif // ADDSERVERPOPUP_H
