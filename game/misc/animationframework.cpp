#include "animationframework.h"

#include <qDebug>

#include "core/window.h"
#include "game/game.h"
#include "game/multiplayergame.h"

extern Window* w;
AnimationFramework::AnimationFramework(float frameTime, int numOfFrames)
{
    Game* game = (Game*)w->getCurrentScene();
    frame = 0;
    this->frameTime = frameTime;
    frameKeeper = game->frameCounter;
    this->numOfFrames = numOfFrames;
    animTime = frameTime * numOfFrames;
}

int AnimationFramework::getAnimationNum(bool reverse)
{
    Game* game = (Game*)w->getCurrentScene();
    if(frameKeeper != game->frameCounter) frame = 0;

    if(reverse) return (numOfFrames+1) - ((frame+animTime/numOfFrames)/(animTime/numOfFrames)); // Give current frame number in opposide order
    return ((frame+animTime/numOfFrames)/(animTime/numOfFrames));
}

void AnimationFramework::update(bool advance)
{
    Game* game = (Game*)w->getCurrentScene();
    if(advance) frame++;
    if(frame == animTime) frame = 0;

    frameKeeper = game->frameCounter + 1;
}
