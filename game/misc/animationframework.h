#ifndef ANIMATIONFRAMEWORK_H
#define ANIMATIONFRAMEWORK_H

class AnimationFramework
{
    int frame; // Holds the current frame
    float frameTime; // Holds long will one frame last
    int numOfFrames; // Holds how many frames does this animation have
    long long frameKeeper; // Holds the game frame and is used for keeping in sync with the game loop
    float animTime; // Holds how long the whole animation will last
public:
    AnimationFramework(float frameTime, int numOfFrames);

    void update(bool advance = true); // Increments frame for one and sets it to 0 if it reaches the animTime
    int getAnimationNum(bool reverse = false); // If the reverse is true it gives numbers in opposite order (3,2,1,0,3...)
};

#endif // ANIMATIONFRAMEWORK_H
