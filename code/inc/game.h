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

enum GameMessages
{
    ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
    ID_PLAYER_INPUT,
    ID_GAME_STATE,
    ID_ASSIGN_PLAYER_INDEX
};

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
    void handleNetwork();
    void startServer(SLNet::RakPeerInterface *server);
    void startClient(SLNet::RakPeerInterface *client);

    bool isServer;
    SLNet::RakPeerInterface *peer;
    std::vector<Player> players;
    std::map<SLNet::SystemAddress, int> playerIndices; // Map to track player indices

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit;
    int frameDelay;
    int clientPlayerIndex; // Index of the player controlled by this client
};

#endif // GAME_H