#include "bottombar.h"

#include <QDebug>
#include <QGraphicsScene>

#include "tileitem.h"
#include "core/window.h"
#include "tilemover.h"
#include "level_creator/levelcreator.h"
#include "level_creator/grid/leveltile.h"
#include "core/inputmanager.h"
#include "main_menu/button.h"
#include "misc/language.h"

extern Window* w;
BottomBar::BottomBar(QRectF rect, QObject *parent) : QObject(parent)
{
    this->setHandlesChildEvents(false);
    this->setPos(rect.x(), rect.y());
    QColor color = QColor(255, 255, 255);
    color.setAlpha(80);
    this->rect = new BorderlessRect(rect.x(), rect.y(), rect.width(), rect.height(), color);
    this->addToGroup(this->rect);

    blockTabButton = new Button(rect.x(), rect.y(), 150, 30, QPixmap(":/images/textures/creator_tab_button_idle.png"), QPixmap(":/images/textures/creator_tab_button_active.png"),
                                 Text(Language::get("blocks"), QColor(0, 0, 0), QFont("VCR OSD Mono", 16)));
    this->addToGroup(blockTabButton);
    this->connect(blockTabButton, SIGNAL(clickEvent()), this, SLOT(blockTabClicked()));

    entityTabButton = new Button(rect.x() + 150, rect.y(), 150, 30, QPixmap(":/images/textures/creator_tab_button_idle.png"), QPixmap(":/images/textures/creator_tab_button_active.png"),
                                 Text(Language::get("entities"), QColor(0, 0, 0), QFont("VCR OSD Mono", 16)));
    this->addToGroup(entityTabButton);
    this->connect(entityTabButton, SIGNAL(clickEvent()), this, SLOT(entityTabClicked()));

    removeButton = new Button(rect.x() + w->getWindowWidth() - 100.0f, rect.y() + 10.0f, 50.0f, 50.0f, QPixmap(":/images/textures/remove.png"), QPixmap(":/images/textures/remove.png"));
    this->addToGroup(removeButton);
    this->connect(removeButton, SIGNAL(clickEvent()), this, SLOT(removeButtonClicked()));

    TileItem* box = new TileItem(QPixmap(":/images/textures/box_tile.png"),
                                 QPointF(rect.x() + 50.0f, rect.y() + 50.0f),
                                 TileItem::TileType::BOX, this);
    this->addToGroup(box);
    this->connect(box, SIGNAL(clickEvent(TileItem*)), this, SLOT(clicked(TileItem*)));
    blocks.push_back(box);

    TileItem* computor = new TileItem(QPixmap(":/images/textures/computor_tile.png"),
                                      QPointF(box->pos().x() + TILE_ITEM_SIZE + 50.0f, rect.y() + 50.0f),
                                      TileItem::TileType::COMPUTER);
    this->addToGroup(computor);
    this->connect(computor, SIGNAL(clickEvent(TileItem*)), this, SLOT(clicked(TileItem*)));
    blocks.push_back(computor);

    TileItem* bookshelf = new TileItem(QPixmap(":/images/textures/bookshelf_tile.png"),
                                       QPointF(computor->pos().x() + TILE_ITEM_SIZE + 50.0f, rect.y() + 50.0f),
                                       TileItem::TileType::BOOKSHELF);
    this->addToGroup(bookshelf);
    this->connect(bookshelf, SIGNAL(clickEvent(TileItem*)), this, SLOT(clicked(TileItem*)));
    blocks.push_back(bookshelf);

    TileItem* playerTile = new TileItem(QPixmap(":/images/textures/player_tile.png"),
                                        QPointF(rect.x() + 50.0f, rect.y() + 50.0f), TileItem::TileType::PLAYER, this);
    this->addToGroup(playerTile);
    playerTile->hide();
    this->connect(playerTile, SIGNAL(clickEvent(TileItem*)), this, SLOT(clicked(TileItem*)));
    entitys.push_back(playerTile);

    TileItem* scientistTile = new TileItem(QPixmap(":/images/textures/scientist_tile.png"),
                                           QPointF(playerTile->pos().x() + TILE_ITEM_SIZE + 50.0f, rect.y() + 50.0f),
                                           TileItem::TileType::SCIENTIST, this);
    this->addToGroup(scientistTile);
    scientistTile->hide();
    this->connect(scientistTile, SIGNAL(clickEvent(TileItem*)), this, SLOT(clicked(TileItem*)));
    entitys.push_back(scientistTile);

    TileItem* zombieTile = new TileItem(QPixmap(":/images/textures/zombie_tile.png"),
                                        QPointF(scientistTile->pos().x() + TILE_ITEM_SIZE + 50.0f, rect.y() + 50.0f),
                                        TileItem::TileType::ZOMBIE, this);
    this->addToGroup(zombieTile);
    zombieTile->hide();
    this->connect(zombieTile, SIGNAL(clickEvent(TileItem*)), this, SLOT(clicked(TileItem*)));
    entitys.push_back(zombieTile);


}

void BottomBar::clicked(TileItem* item)
{
    QPointF mousePos = w->mapToScene(w->getInputManager()->mousePosition.toPoint());
    LevelTile* tile = new LevelTile(item, QPointF(mousePos.x() - (TILE_ITEM_SIZE/2.0f),
                                                  mousePos.y() - (TILE_ITEM_SIZE/2.0f)));
    w->getLevelCreator()->addItem(tile);
    w->getLevelCreator()->getMover()->setTile(tile);
}

void BottomBar::blockTabClicked()
{
    if(blocks.first()->isVisible()) return;

    for(int i = 0; i < entitys.length(); i++)
        entitys[i]->hide();

    for(int i = 0; i < blocks.length(); i++)
        blocks[i]->show();
}

void BottomBar::entityTabClicked()
{
    if(entitys.first()->isVisible()) return;

    for(int i = 0; i < blocks.length(); i++)
        blocks[i]->hide();

    for(int i = 0; i < entitys.length(); i++)
        entitys[i]->show();
}

void BottomBar::removeButtonClicked()
{
    w->setCursor(QCursor(removeButton->getIdlePixmap()));
    w->getLevelCreator()->setRemoveMode(true);
}

QPixmap BottomBar::getRemovePixmap()
{
    return removeButton->getIdlePixmap();
}

void BottomBar::update()
{
}
