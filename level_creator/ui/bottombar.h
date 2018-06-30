#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include "main_menu/borderlessrect.h"
#include "game/ui/uicomponent.h"

class TileItem;
class Button;

class BottomBar : public QObject, public UIComponent
{
    Q_OBJECT

    BorderlessRect* rect;
    Button* blockTabButton;
    Button* entityTabButton;
    Button* removeButton;
    QList<TileItem*> blocks;
    QList<TileItem*> entitys;
public slots:
    void clicked(TileItem* item);
    void blockTabClicked();
    void entityTabClicked();
    void removeButtonClicked();
public:
    BottomBar(QRectF rect, QObject* parent = nullptr);

    void update();

    QPixmap getRemovePixmap();
};

#endif // BOTTOMBAR_H
