#include "gameClient.h"
#include "player.h"

#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()
#include <iostream>

#include <slikenet/sleep.h>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;

GameClient::GameClient(SLNet::RakPeerInterface *peer)
    : GameBase(peer)
{
}

bool GameClient::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("blockGame Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SLNet::BitStream bsOut; // client asks for all players
    bsOut.Write((SLNet::MessageID)ID_SEND_PLAYERS);
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);

    bool gameReady = false;
    while (!gameReady)
    {
        for (SLNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) // client receives all players and wait for game ready
        {
            switch (packet->data[0])
            {
            case ID_GAME_READY:
                std::cout << "Client Received a message with identifier " << "ID_GAME_READY" << std::endl;
                gameReady = true;
                break;

            case ID_PLAYER_INIT:
            {
                std::cout << "Client Received a message with identifier " << "ID_PLAYER_INIT" << std::endl;
                PlayerInit playerInit;
                SLNet::BitStream bsIn(packet->data, packet->length, false);
                bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
                bsIn.Read(playerInit);

                std::cout << "player colorRed: " << playerInit.colorRed << std::endl;
                std::cout << "player colorGreen: " << playerInit.colorGreen << std::endl;
                std::cout << "player colorBlue: " << playerInit.colorBlue << std::endl;
                std::cout << "player posX: " << playerInit.posX << std::endl;
                std::cout << "player posY: " << playerInit.posY << "\n"
                          << std::endl;

                SDL_Color color = {playerInit.colorRed, playerInit.colorGreen, playerInit.colorBlue};
                Player player(playerInit.posX, playerInit.posY, color);
                players.push_back(player);
                break;
            }

            default:
                std::cout << "Received a message with identifier " << "Default" << std::endl;
                break;
            }
        }
    }

    bsOut.Reset();
    bsOut.Write((SLNet::MessageID)ID_SEND_PLAYER_ID); // client asks for his ID
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);

    gameReady = false;
    while (!gameReady)
    {
        for (SLNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) // client receives his ID and wait for game ready
        {
            switch (packet->data[0])
            {
            case ID_GAME_READY:
                std::cout << "Client Received a message with identifier " << "ID_GAME_READY" << std::endl;
                gameReady = true;
                break;
            case ID_PLAYER_STATE:
            {
                std::cout << "Client Received a message with identifier " << "ID_PLAYER_STATE" << std::endl;
                PlayerState playerState;
                SLNet::BitStream bsIn(packet->data, packet->length, false);
                bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
                bsIn.Read(playerState);
                std::cout << "player ID: " << playerState.playerId << std::endl;
                std::cout << "player posX: " << playerState.posX << std::endl;
                std::cout << "player posY: " << playerState.posY << "\n"
                          << std::endl;
                break;
            }
            default:
                std::cout << "Received a message with identifier " << "default" << std::endl;
                break;
            }
        }
    }

    return true;
}

void GameClient::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
    }
    // Client reads all key inputs
    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
    PlayerInput playerInput;
    playerInput.up = currentKeyStates[SDL_SCANCODE_UP];
    playerInput.down = currentKeyStates[SDL_SCANCODE_DOWN];
    playerInput.left = currentKeyStates[SDL_SCANCODE_LEFT];
    playerInput.right = currentKeyStates[SDL_SCANCODE_RIGHT];
    playerInput.inputSequenceNumber = 0; // TODO: Implement input sequence number

    SLNet::BitStream bsOut; // client sends all key inputs
    bsOut.Write((SLNet::MessageID)ID_PLAYER_INPUT);
    bsOut.Write(playerInput);
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void GameClient::handleNetwork()
{
    for (SLNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_CONNECTION_REQUEST_ACCEPTED:
        {

            std::cout << "Our connection request has been accepted.\n";
            break;
        }
        case ID_PLAYER_STATE:
        {
            std::cout << "Received a message with identifier ID_PLAYER_STATE" << std::endl;
            PlayerState playerState;
            SLNet::BitStream bsIn(packet->data, packet->length, false);
            bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
            bsIn.Read(playerState);
            std::cout << "player ID: " << playerState.playerId << std::endl;
            std::cout << "player posX: " << playerState.posX << std::endl;
            std::cout << "player posY: " << playerState.posY << "\n"
                      << std::endl;

            players[playerState.playerId].setPosition(playerState.posX, playerState.posY);

            break;
        }
        default:
            std::cout << "Received a message with identifier " << "Default" << std::endl;
            break;
        }
    }
}