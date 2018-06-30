#ifndef PLAYER_H
#define PLAYER_H

#include <QPointF>
#include <QGraphicsTextItem>

#include "mob.h"

enum class PlayerWalkState
{
    WALK_FORWARD,
    WALK_FORWARD_LEFT,
    WALK_FORWARD_RIGHT,
    WALK_BACKWARD,
    WALK_BACKWARD_LEFT,
    WALK_BACKWARD_RIGHT,
    WALK_LEFT,
    WALK_RIGHT
};

class InputManager;
class AnimationFramework;
class Gun;

class Player : public QObject, public Mob
{
    Q_OBJECT

    AnimationFramework* animFW[8];
    AnimationFramework* shootingAnimFW;
    Gun* gun;
protected:
    bool isMoving;
    bool shooting;
    bool reverseAnim;
    bool resetAnim;
    PlayerWalkState state;
    float degrees;
    QGraphicsTextItem* nameTag;
    bool infected;
    void setPlayerWalkState(); // This checks the players orientation (degrees) and sets the PlayerWalkState base on that
signals:
    void shotBullet();
public:
    Player(QPointF position);
    virtual ~Player();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void handleInput(); // Process input based on the InputManager
    void update(); // Update players gun
    void entityCollision(); // Just do Mob::mobCollision()
    void enableNameTag(QString username); // This sets the players nametag equal to the QString
    void updateNameTag(); // This changes the nametag position relative to players position

    PlayerWalkState getPlayerWalkState() { return state; }
    Gun* getCurrentGun() { return gun; }
    float getDegrees() { return degrees; }
    void setInfected(bool infected) { this->infected = infected; }
    bool isShooting() { return shooting; }
    bool isInfected() { return infected; }
};

#endif // PLAYER_H
