#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QMap>
#include <QPointF>

class InputManager
{   
    QMap<int, bool> keyMap; // We store which keys are pressed [keyValue][isPressed/isNotPressed]
public:
    InputManager();

    void pressKey(int keyID); // Sets the key in the keyMap to true
    void releaseKey(int keyID); // Sets the key in the keyMap to false

    bool isKeyPressed(int keyID); // Tells us if the key is currently pressed

    QPointF mousePosition;
    bool mouseButtonPressed;
};

#endif // INPUTMANAGER_H
