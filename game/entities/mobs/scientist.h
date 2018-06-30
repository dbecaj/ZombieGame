#ifndef CIVILIAN_H
#define CIVILIAN_H

#include <QPointF>

#include "mob.h"
#include "game/misc/animationframework.h"

enum class ScientistGender
{
    MALE,
    FEMALE
};

class Scientist : public Mob
{
    static long idIncrement; // This is just for giving different ids to all scientists

    int frameTimer;
    ScientistGender gender;
    AnimationFramework* animFW[4];
    long id;
protected:
    float angle;
    float speed;

    void setScientistWalkState(); // This checks the scientists orientation (degrees) and sets the MobWalkState base on that
public:
    Scientist(ScientistGender gender, QPointF position, long id = -1);
    virtual ~Scientist();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void update(); // Updates the direction if need be and moves the player in the set direction
    void randomizeDirection(); // Picks a random direction between 0.0f and 360.0f
    long getId() { return id; }
    ScientistGender getGender() { return gender; }
    float getDirection() { return angle; }
};

#endif // CIVILIAN_H
