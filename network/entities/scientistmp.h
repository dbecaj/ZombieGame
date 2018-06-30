#ifndef SCIENTISTMP_H
#define SCIENTISTMP_H

#include "game/entities/mobs/scientist.h"

class ScientistMP : public Scientist
{
public:
    ScientistMP(ScientistGender gender, QPointF position, long id);

    void update(); // Just moves scientist in a direction like a normal scientist with the exception that the direction does not change automatically
    void updatePos(float x, float y, float dir); // Correct the position if need be and set new direction
    void changeAngle(float angle);
};

#endif // SCIENTISTMP_H
