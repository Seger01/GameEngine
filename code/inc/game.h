#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <slikenet/peerinterface.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <slikenet/PacketLogger.h>
#include <vector>
#include <deque>
#include "player.h"

enum GameMessages
{
    ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
    ID_PLAYER_INPUT,
    ID_PLAYER_STATE,
    ID_PLAYER_INIT,
    ID_GAME_READY,
    ID_SEND_PLAYERS,
    ID_SEND_PLAYER_ID
};

struct PlayerInput
{
    int playerId;
    bool up;
    bool down;
    bool left;
    bool right;
    uint32_t inputSequenceNumber; // Helps for client prediction
};

struct PlayerState
{
    int playerId;
    int posX;
    int posY;
    uint32_t inputSequenceNumber; // Helps for client prediction
};

struct PlayerInit
{
    uint8_t colorRed;
    uint8_t colorGreen;
    uint8_t colorBlue;
    int posX;
    int posY;
};

class Game
{
public:
    Game(bool isServer, SLNet::RakPeerInterface *peer);
    bool init();
    void run();
    void cleanup();

private:
    void handleEvents();
    void handleNetwork();
    void update();
    void render();
    void handleNetwork();
    void startServer(SLNet::RakPeerInterface *server);
    void startClient(SLNet::RakPeerInterface *client);

    bool isServer;
    SLNet::RakPeerInterface *peer;
    std::vector<Player> players;
    std::deque<PlayerInput> inputHistory; // History of inputs for reconciliation
    int sequenceNumber;                   // Sequence number for inputs

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit;
    int frameDelay;

    int playerID;
    std::vector<Player> players;

    bool isServer;
    SLNet::RakPeerInterface *peer;
};

#endif // GAME_H