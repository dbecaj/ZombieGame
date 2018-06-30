#ifndef PLAYERMP_H
#define PLAYERMP_H

#include <QHostAddress>

#include "game/entities/mobs/player.h"
#include "game/entities/bullet.h"

class PlayerMP : public Player
{
    QString username;

    QPointF position; // Temporary stores the recived player position
public:
    PlayerMP(QPointF position, QString username);

    void updatePosition(QPointF position, float degrees); // This updates the position and the facing direction
    void fireBullet(); // Fire a bullet
    void update(); // Does update on position(if we moved) and on other player components

    QString getUsername() { return username; }
};

#endif // PLAYERMP_H
