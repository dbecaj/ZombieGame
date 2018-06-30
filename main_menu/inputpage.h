#ifndef INPUTPAGE_H
#define INPUTPAGE_H

#include "core/scene.h"

class InsertText;

class InputPage : public Scene
{
    InsertText* insertText; // Our custom input class

    void keyPressEvent(QKeyEvent *event);
public:
    explicit InputPage(QObject* parent = nullptr);

    void init();
    void cleanUp();
};

#endif // INPUTPAGE_H
