#ifndef GUN_H
#define GUN_H

#include <QList>
#include <QObject>
#include <QSoundEffect>

#include "game/entities/mobs/mob.h"

class Bullet;
class Player;

class Gun : public QObject
{
    Q_OBJECT

    int cooldown;
    Player* owner;
    QList<Bullet*> bullets;
    QList<QSoundEffect*> shotSounds;

    bool handleSingleBulletCollision(Bullet* bullet); // This is just a helper funtion in bullet collision
signals:
    void mobHit(Mob* mob);
public:
    explicit Gun(Player* owner, QObject* parent = nullptr);
    ~Gun();

    bool fire(bool ignoreCooldown = false); // Fires a bullet in the players direction
    void update(); // Updates all bullets and decrements the cooldown
    void handleBulletCollision(); // Handles the collision and destruction of each bullet

    QList<Bullet*> getBullets() { return bullets; }
    void removeAllBullets();
};

#endif // GUN_H
