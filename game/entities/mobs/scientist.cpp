#include "scientist.h"

#include <QPixmap>
#include <random>
#include <ctime>
#include <qmath.h>
#include <QDebug>
#include <QPainter>

#include "game/game.h"
#include "core/window.h"

extern Window* w;
Scientist::Scientist(ScientistGender gender, QPointF position, long id) : Mob(position, MobType::SCIENTIST)
{
    // FRAME_TIME tells us how many game frames does each animation frame last
    static const float FRAME_TIME = 7.0f;
    static const int NUM_OF_FRAMES = 4;
    this->gender = gender;
    speed = 8.0f;
    frameTimer = 0;

    // Load the scientist sprite map base on the gender
    QString path;
    if(gender == ScientistGender::MALE) path = ":/images/textures/scientist_man_sprite.png";
    else path = ":/images/textures/scientist_woman_sprite.png";
    if(!pixmap.load(path)) qDebug() << "Civilian>failed to load texture with path: " << path;

    // Set random direction and based on that set the walkState
    randomizeDirection();
    setScientistWalkState();

    // Initialize all animFWs
    for(int i = 0; i < 4; i++)
        animFW[i] = new AnimationFramework(FRAME_TIME, NUM_OF_FRAMES);

    // id default is -1
    if(id >= 0) this->id = id;
    else this->id = idIncrement++;
}

long Scientist::idIncrement = 0;

Scientist::~Scientist()
{
    // Delete all objects that are not covered by the QT framework
    for(int i = 0; i < 4; i++)
        delete animFW[i];
}

void Scientist::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // We do the standard mob pain which only paints a rect around the boundingBox and shape if the debug is true
    Mob::paint(painter, option, widget);

    static const QPointF dims(33.0f, 51.0f); // This dimensions are for MALE scientists
    static const QPointF dims2(16.0f, 29.0f); // This are for FEMALE scientists
    QRectF targetRect, sourceRect; // targetRect is where we will paint (boundingRect()) and sourceRect is the sprite that we will draw from the sprite map
    //targetRect.setRect(0.0f, MOB_HEIGHT-80.0f, MOB_WIDTH, 80.0f);
    targetRect = this->boundingRect();

    // Base on gender we use different coords for different sprite maps
    if(gender == ScientistGender::MALE)
    {
        switch(walkState)
        {
            case MobWalkState::WALK_FORWARD:
            {
                static const float Y = 0.0f;

                int animNum = animFW[0]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(0.0f, Y, dims.x(), dims.y());
                else if(animNum == 2) sourceRect.setRect(36.0f, Y, dims.x(), dims.y());
                else if(animNum == 3) sourceRect.setRect(70.0f, Y, dims.x(), dims.y());
                else sourceRect.setRect(104.0f, Y, dims.x(), dims.y());

                // We update the animation framework on that index so that the animation flows and the animationNum changes every NUM_OF_FRAMES
                animFW[0]->update();
                break;
            }
            case MobWalkState::WALK_BACKWARD:
            {
                static const float Y = 156.0f;

                int animNum = animFW[1]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(0.0f, Y, dims.x(), dims.y());
                else if(animNum == 2) sourceRect.setRect(36.0f, Y, dims.x(), dims.y());
                else if(animNum == 3) sourceRect.setRect(70.0f, Y, dims.x(), dims.y());
                else sourceRect.setRect(104.0f, Y, dims.x(), dims.y());

                animFW[1]->update();
                break;
            }
            case MobWalkState::WALK_LEFT:
            {
                static const float Y = 52.0f;

                int animNum = animFW[2]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(0.0f, Y, dims.x(), dims.y());
                else if(animNum == 2) sourceRect.setRect(36.0f, Y, dims.x(), dims.y());
                else if(animNum == 3) sourceRect.setRect(70.0f, Y, dims.x(), dims.y());
                else sourceRect.setRect(104.0f, Y, dims.x(), dims.y());

                animFW[2]->update();
                break;
            }
            case MobWalkState::WALK_RIGHT:
            {
                static const float Y = 104.0f;

                int animNum = animFW[3]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(0.0f, Y, dims.x(), dims.y());
                else if(animNum == 2) sourceRect.setRect(36.0f, Y, dims.x(), dims.y());
                else if(animNum == 3) sourceRect.setRect(70.0f, Y, dims.x(), dims.y());
                else sourceRect.setRect(104.0f, Y, dims.x(), dims.y());

                animFW[3]->update();
                break;
            }
        }
    }
    else // FEMALE scientist
    {
        switch(walkState)
        {
            case MobWalkState::WALK_FORWARD:
            {
                static const float Y = 168.0f;

                int animNum = animFW[0]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(11.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 2) sourceRect.setRect(52.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 3) sourceRect.setRect(92.0f, Y, dims2.x(), dims2.y());
                else sourceRect.setRect(132.0f, Y, dims2.x(), dims2.y());

                animFW[0]->update();
                break;
            }
            case MobWalkState::WALK_BACKWARD:
            {
                static const float Y = 288.0f;

                int animNum = animFW[1]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(11.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 2) sourceRect.setRect(52.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 3) sourceRect.setRect(92.0f, Y, dims2.x(), dims2.y());
                else sourceRect.setRect(132.0f, Y, dims2.x(), dims2.y());

                animFW[1]->update();
                break;
            }
            case MobWalkState::WALK_LEFT:
            {
                static const float Y = 208.0f;

                int animNum = animFW[2]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(11.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 2) sourceRect.setRect(52.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 3) sourceRect.setRect(92.0f, Y, dims2.x(), dims2.y());
                else sourceRect.setRect(132.0f, Y, dims2.x(), dims2.y());

                animFW[2]->update();
                break;
            }
            case MobWalkState::WALK_RIGHT:
            {
                static const float Y = 248.0f;

                int animNum = animFW[3]->getAnimationNum();
                if(animNum == 1) sourceRect.setRect(11.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 2) sourceRect.setRect(52.0f, Y, dims2.x(), dims2.y());
                else if(animNum == 3) sourceRect.setRect(92.0f, Y, dims2.x(), dims2.y());
                else sourceRect.setRect(132.0f, Y, dims2.x(), dims2.y());

                animFW[3]->update();
                break;
            }
        }
    }
    // We just draw a correct sprite of the scientist (sourceRect) in the boundingBox(targetRect)
    painter->drawPixmap(targetRect, pixmap, sourceRect);
}

void Scientist::update()
{
    // Every 60 game frames or when we are colliding with a wall we change the direction
    if(frameTimer >= 60 || colliding)
    {
        randomizeDirection();
        frameTimer = 0;
    }

    frameTimer++;

    // Move the scientist in the set direction
    float rad = qDegreesToRadians(angle);
    this->setX(this->x() + qCos(rad) * speed);
    this->setY(this->y() + qSin(rad) * speed);

    setScientistWalkState();
}

void Scientist::randomizeDirection()
{
    static std::mt19937 randEngine(time(nullptr));
    std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
    angle = randAngle(randEngine);
}

void Scientist::setScientistWalkState()
{
    if(angle >= 45.0f && angle <= 135.0f) walkState = MobWalkState::WALK_FORWARD;
    else if(angle >= 135.0f && angle <= 225.0f) walkState = MobWalkState::WALK_LEFT;
    else if(angle >= 225.0f && angle <= 315.0f) walkState = MobWalkState::WALK_BACKWARD;
    else walkState = MobWalkState::WALK_RIGHT;
}
