#include "game.h"
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

Game::Game(bool isServer, SLNet::RakPeerInterface *peer)
    : window(NULL), renderer(NULL), quit(false), frameDelay(1000 / 60), playerID(-1)
{
    this->isServer = isServer;
    this->peer = peer;
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    std::string windowName;
    if (isServer)
    {
        windowName = "blockGame Server";
    }
    else
    {
        windowName = "blockGame Client";
    }

    window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    if (!isServer) // Check if it is a client
    {
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

                    this->playerID = playerState.playerId;
                    break;
                }
                default:
                    std::cout << "Received a message with identifier " << "default" << std::endl;
                    break;
                }
            }
        }
    }

    return true;
}

void Game::run()
{
    while (!quit)
    {
        Uint32 frameStart = SDL_GetTicks();

        handleEvents();
        handleNetwork();
        update();
        render();

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
    }
    if (!isServer) // check if server or client
    {
        // Client reads all key inputs
        const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
        PlayerInput playerInput;
        playerInput.playerId = this->playerID;
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
}

void Game::handleNetwork()
{
    for (SLNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_NEW_INCOMING_CONNECTION:
        {
            if (isServer)
            {
                std::cout << "A connection is incoming.\n";
            }
            break;
        }
        case ID_CONNECTION_REQUEST_ACCEPTED:
        {
            if (!isServer)
            {
                std::cout << "Our connection request has been accepted.\n";
            }
            break;
        }
        case ID_SEND_PLAYERS:
        {
            std::cout << "Received a message with identifier ID_SEND_PLAYERS" << std::endl;
            for (int i = 0; i < players.size(); i++) // Server sends all players to client
            {
                PlayerInit playerInit;
                playerInit.colorRed = players[i].getColor().r;
                playerInit.colorGreen = players[i].getColor().g;
                playerInit.colorBlue = players[i].getColor().b;
                playerInit.posX = players[i].getX();
                playerInit.posY = players[i].getY();

                SLNet::BitStream bsOut;
                bsOut.Write((SLNet::MessageID)ID_PLAYER_INIT);
                bsOut.Write(playerInit);
                peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
            }

            SLNet::BitStream bsOut; // Server sends game ready to client
            bsOut.Write((SLNet::MessageID)ID_GAME_READY);
            peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

            break;
        }
        case ID_SEND_PLAYER_ID:
        {
            std::cout << "Received a message with identifier ID_SEND_PLAYER_ID" << std::endl;
            SLNet::BitStream bsOut; // Server sends player ID to client
            bsOut.Write((SLNet::MessageID)ID_PLAYER_STATE);
            PlayerState playerState;
            playerState.playerId = players.size() - 1;
            bsOut.Write(playerState);
            peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

            bsOut.Reset(); // Server sends game ready to client
            bsOut.Write((SLNet::MessageID)ID_GAME_READY);
            peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

            break;
        }
        case ID_PLAYER_INPUT:
        {
            std::cout << "Received a message with identifier ID_PLAYER_INPUT" << std::endl;
            if (isServer)
            {
                PlayerInput playerInput;
                SLNet::BitStream bsIn(packet->data, packet->length, false);
                bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
                bsIn.Read(playerInput);
                std::cout << "player ID: " << playerInput.playerId << std::endl;
                std::cout << "player up: " << playerInput.up << std::endl;
                std::cout << "player down: " << playerInput.down << std::endl;
                std::cout << "player left: " << playerInput.left << std::endl;
                std::cout << "player right: " << playerInput.right << "\n"
                          << std::endl;

                for (int i = 0; i < players.size(); i++) // Server updates all player locations
                {
                    if (i == playerInput.playerId)
                    {
                        players[i].handleInput(playerInput.up, playerInput.down, playerInput.left, playerInput.right);
                    }
                }

                PlayerState playerState;
                for (int i = 0; i < players.size(); i++) // Server sends all player locations
                {
                    players[i].update();

                    playerState.playerId = i;
                    playerState.posX = players[i].getX();
                    playerState.posY = players[i].getY();

                    SLNet::BitStream bsOut;
                    bsOut.Write((SLNet::MessageID)ID_PLAYER_STATE);
                    bsOut.Write(playerState);
                    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
                }
            }
            break;
        }
        case ID_PLAYER_STATE:
        {
            std::cout << "Received a message with identifier ID_PLAYER_STATE" << std::endl;
            if (!isServer)
            {

                PlayerState playerState;
                SLNet::BitStream bsIn(packet->data, packet->length, false);
                bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
                bsIn.Read(playerState);
                std::cout << "player ID: " << playerState.playerId << std::endl;
                std::cout << "player posX: " << playerState.posX << std::endl;
                std::cout << "player posY: " << playerState.posY << "\n"
                          << std::endl;

                players[playerState.playerId].setPosition(playerState.posX, playerState.posY);
            }
            break;
        }
        case ID_PLAYER_INIT:
        {
            std::cout << "Received a message with identifier ID_PLAYER_INIT" << std::endl;
            if (isServer)
            {
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
                players.push_back(Player(playerInit.posX, playerInit.posY, {playerInit.colorRed, playerInit.colorGreen, playerInit.colorBlue}));
            }
            break;
        }
        default:
            std::cout << "Received a message with identifier " << "Default" << std::endl;
            break;
        }
    }
}

void Game::update()
{
    for (auto &player : players)
    {
        player.update();
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Rect fillRect = {1, 1, 3, 3};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer, &fillRect);

    for (auto &player : players)
    {
        player.render(renderer);
    }

    SDL_RenderPresent(renderer);
}