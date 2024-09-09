#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include "player.h"
#include <slikenet/peerinterface.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <slikenet/PacketLogger.h>

class Game
{
public:
    Game();
    bool init();
    void run();
    void cleanup();

public:
    bool isServer;

private:
    void startServer(SLNet::RakPeerInterface *server);
    void startClient(SLNet::RakPeerInterface *client);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit;
    const int frameDelay;
    std::vector<Player> players;

    SLNet::RakPeerInterface *peer;

    void handleEvents();
    void update();
    void render();
    void handleNetwork();
};

#endif // GAME_H