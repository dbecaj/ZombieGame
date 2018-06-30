#ifndef ZOMBIEMP_H
#define ZOMBIEMP_H

#include "game/entities/mobs/zombie.h"

class ZombieMP : public Zombie
{
public:
    ZombieMP(QPointF position, long id);

    void update(); // Set the direction towards the target mob and move to it by speed
    void setTarget(Mob *target); // Change the target mob

    void mobCollision();
};

#endif // ZOMBIEMP_H
