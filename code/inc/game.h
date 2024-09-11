#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <slikenet/peerinterface.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <slikenet/PacketLogger.h>
#include <vector>
#include <map>
#include "player.h"

class Game
{
public:
    Game();
    bool init();
    void run();
    void cleanup();
    void handleEvents();
    void update();
    void render();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit;
    int frameDelay;
    std::vector<Player> players;
};

#endif // GAME_H