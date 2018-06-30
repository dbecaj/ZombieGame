#ifndef SPECTATOR_H
#define SPECTATOR_H


class Spectator
{
    int playerIndex;
public:
    Spectator();

    void update();
    bool next();
};

#endif // SPECTATOR_H
