#include "inputmanager.h"

#include <QDebug>

InputManager::InputManager()
{
    mousePosition = QPointF(0.0f, 0.0f);
    mouseButtonPressed = false;
}

void InputManager::pressKey(int keyID)
{
    // If the keyId does not exist in the keyMap it automatically creates one
    keyMap[keyID] = true;
}

void InputManager::releaseKey(int keyID)
{
    // Same here
    keyMap[keyID] = false;
}

bool InputManager::isKeyPressed(int keyID)
{
    auto it = keyMap.find(keyID);
    if(it != keyMap.end()) return it.value(); // if the iterator didn't come to the end (we found the keyId in keyMap) we return the value that is stored in that place
    else return false;
}
