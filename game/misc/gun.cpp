#include "gun.h"

#include <random>
#include <ctime>

#include "game/entities/bullet.h"
#include "game/entities/mobs/player.h"
#include "core/window.h"
#include "game/multiplayergame.h"
#include "game/game.h"
#include "tile.h"

extern Window* w;
Gun::Gun(Player *owner, QObject* parent) : QObject(parent)
{
    this->owner = owner;
    cooldown = 0;

    QSoundEffect* sound = new QSoundEffect(this);
    sound->setSource(QUrl("qrc:/sound/sound/shot.wav"));
    sound->setVolume(0.2f);
    shotSounds.push_back(sound);
}

Gun::~Gun()
{
    qDebug() << "deleting gun!";

    bullets.clear();
    qDebug() << "gun done!";
}

bool Gun::fire(bool ignoreCooldown)
{
    static std::mt19937 randEngine(time(nullptr));
    if(cooldown != 0 && !ignoreCooldown) return false;

    Direction dir;
    QPointF pos;
    switch(owner->getPlayerWalkState())
    {
        case PlayerWalkState::WALK_FORWARD:
        {
            dir = Direction::DOWN;
            pos = QPointF(owner->pos().x() + MOB_WIDTH/2.0f - 10.0f, owner->pos().y() + MOB_HEIGHT/2.0f - 5.0f);
            break;
        }
        case PlayerWalkState::WALK_FORWARD_LEFT:
        {
            dir = Direction::DOWN_LEFT;
            pos = QPointF(owner->pos().x(), owner->pos().y() + MOB_HEIGHT/2.0f);
            break;
        }
        case PlayerWalkState::WALK_FORWARD_RIGHT:
        {
            dir = Direction::DOWN_RIGHT;
            pos = QPointF(owner->pos().x() + MOB_WIDTH, owner->pos().y() + MOB_HEIGHT/2.0f);
            break;
        }
        case PlayerWalkState::WALK_BACKWARD:
        {
            dir = Direction::UP;
            pos = QPointF(owner->pos().x() + MOB_WIDTH - 22.0f, owner->pos().y());
            break;
        }
        case PlayerWalkState::WALK_BACKWARD_LEFT:
        {
            dir = Direction::UP_LEFT;
            pos = QPointF(owner->pos().x(), owner->pos().y() + MOB_HEIGHT/2.0f);
            break;
        }
        case PlayerWalkState::WALK_BACKWARD_RIGHT:
        {
            dir = Direction::UP_RIGHT;
            pos = QPointF(owner->pos().x() + MOB_WIDTH, owner->pos().y() + MOB_HEIGHT/2.0f);
            break;
        }
        case PlayerWalkState::WALK_LEFT:
        {
            dir = Direction::LEFT;
            pos = QPointF(owner->pos().x(), owner->pos().y() + MOB_HEIGHT/2.0f - 6.0f);
            break;
        }
        case PlayerWalkState::WALK_RIGHT:
        {
            dir = Direction::RIGHT;
            pos = QPointF(owner->pos().x() + MOB_WIDTH, owner->pos().y() + MOB_HEIGHT/2.0f - 6.0f);
            break;
        }
    }

    Bullet* bullet = new Bullet(pos, dir);
    bullets.push_back(bullet);
    w->scene()->addItem(bullet);
    cooldown = 20;

    shotSounds[0]->play();

    return true;
}

void Gun::update()
{
    if(cooldown > 0) cooldown--;
    for(int i = 0; i < bullets.length(); i++)
        bullets[i]->update();
}

bool Gun::handleSingleBulletCollision(Bullet *bullet)
{
    QRectF bulletBox(bullet->pos().x(), bullet->pos().y(), BULLET_SIZE, BULLET_SIZE);
    Game* game = (Game*)w->getCurrentScene();
    QList<Mob*> mobs = game->getMobs();
    for(int i = 0; i < mobs.length(); i++)
    {
        QRectF mobBox(mobs[i]->pos().x(), mobs[i]->pos().y(), MOB_WIDTH, MOB_HEIGHT);
        if(!bulletBox.intersects(mobBox)) continue;

        if(mobs[i]->getMobType() == MobType::PLAYER) return false;
        emit mobHit(mobs[i]);
        return true;
    }

    QList<Tile*> tiles = game->tiles;
    for(int i = 0; i < tiles.length(); i++)
    {
        QRectF tileBox(tiles[i]->pos().x(), tiles[i]->pos().y(), TILE_SIZE, TILE_SIZE);
        if(!bulletBox.intersects(tileBox)) continue;

        return true;
    }

    return false;
}

void Gun::handleBulletCollision()
{
    for(Bullet* b : bullets)
    {
        if(handleSingleBulletCollision(b)) // If collision is detected by a ENTITY or a MOB delete bullet
        {
            delete b;
            bullets.removeOne(b);
        }
    }
}

void Gun::removeAllBullets()
{
    for(int i = 0; i < bullets.length(); i++)
        delete bullets[i];

    bullets.clear();
}
