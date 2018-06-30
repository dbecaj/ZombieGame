#include "spectator.h"

#include "core/window.h"
#include "game/multiplayergame.h"
#include "network/entities/playermp.h"

extern Window *w;
Spectator::Spectator()
{
    playerIndex = 0;
}

void Spectator::update()
{
    QList<PlayerMP*> players = w->getMultiplayerGame()->getPlayers();
    if(players.empty()) return;

    if(playerIndex >= players.length()) playerIndex = 0;

    w->centerOn(players.at(playerIndex));
}

bool Spectator::next()
{
    QList<PlayerMP*> players = w->getMultiplayerGame()->getPlayers();
    if(players.length() == (playerIndex + 1))
    {
        playerIndex = 0;
        return false;
    }

    playerIndex++;
    return true;
}
