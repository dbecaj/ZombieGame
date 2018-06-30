#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <QPointF>
#include <QVector2D>
#include <QGraphicsItem>
#include <QPixmap>
#include <QPainterPath>
#include <QRectF>
#include <QSoundEffect>
#include <QTimer>

#include "mob.h"
#include "game/misc/animationframework.h"

class QPainter;

class Zombie : public QObject, public Mob
{   
    Q_OBJECT

    static long idIncrement; // This is just for giving different ids to all zombies

    AnimationFramework* animFW[4];
    long id;
    static QSoundEffect attackedSound;
    int lives;
protected:
    Mob* target; // The mob that the zombie is chasing
    float speed;
    float angle;

    void setZombieWalkState(); // Sets zombie walkState based on the angle
signals:
    void mobHit(Mob* mob);
public:
    Zombie(QPointF position, long id = -1);
    virtual ~Zombie();

    virtual void update(); // Finds the closest mob that is PLAYER or SCIENITIST and sets the target to it
    Mob* scanForTarget(); // Goes through all scientists and player and returns the closest one
    void damage(int ammount);
    void playAttackedSound();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mobCollision(); // This sends the mobHit signal if it hits PLAYER or SCIENTIST

    long getId() { return id; }
    int getLives() { return lives; }
    Mob* getTarget() { return target; }
    void setLives(int lives) { this->lives = lives; }
};

#endif // ZOMBIE_H
