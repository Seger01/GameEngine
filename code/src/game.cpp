#include "game.h"
#include <iostream>
#include "player.h"
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;
#define SERVER_PORT 60000
#define SERVER_IP "127.0.0.1"

Game::Game()
    : window(NULL), renderer(NULL), quit(false), frameDelay(1000 / 60), peer(NULL), isServer(false), clientPlayerIndex(-1)
{ // Seed the random number generator
    std::srand(std::time(0));
}

bool Game::init()
{
    std::string windowName;
    if (isServer)
    {
        windowName = "Server mode";
    }
    else
    {
        windowName = "Client mode";
    }
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
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

    // Initialize SLikeNet
    peer = SLNet::RakPeerInterface::GetInstance();
    SLNet::SocketDescriptor sd;
    if (isServer)
    {
        startServer(peer);
    }
    else
    {
        startClient(peer);
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

    SLNet::RakPeerInterface::DestroyInstance(peer);
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

    if (!isServer)
    {
        // Handle input for the client player
        players[0].handleInput(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);
    }
}

void Game::update()
{
    if (isServer)
    {
        for (auto &player : players)
        {
            player.update();
        }

        // Send game state to clients
        SLNet::BitStream bsOut;
        bsOut.Write((SLNet::MessageID)ID_GAME_STATE);
        for (const auto &player : players)
        {
            bsOut.Write(player.getX());
            bsOut.Write(player.getY());
            bsOut.Write(player.getColor().r);
            bsOut.Write(player.getColor().g);
            bsOut.Write(player.getColor().b);
        }
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    for (auto &player : players)
    {
        player.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::handleNetwork()
{
    for (SLNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_NEW_INCOMING_CONNECTION:
            if (isServer)
            {
                std::cout << "A connection is incoming.\n";
                // Generate random color for the new player
                Uint8 r = rand() % 256;
                Uint8 g = rand() % 256;
                Uint8 b = rand() % 256;
                // Add a new player for the incoming connection with a random color
                players.push_back(Player(100, 100, {r, g, b}));
                int playerIndex = players.size() - 1;
                playerIndices[packet->systemAddress] = playerIndex;

                // Send the player index to the client
                SLNet::BitStream bsOut;
                bsOut.Write((SLNet::MessageID)ID_ASSIGN_PLAYER_INDEX);
                bsOut.Write(playerIndex);
                peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
            }
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            if (!isServer)
            {
                std::cout << "Our connection request has been accepted.\n";
            }
            break;
        case ID_ASSIGN_PLAYER_INDEX:
            if (!isServer)
            {
                SLNet::BitStream bsIn(packet->data, packet->length, false);
                bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
                bsIn.Read(clientPlayerIndex);
                std::cout << "Assigned player index: " << clientPlayerIndex << std::endl;
            }
            break;
        case ID_PLAYER_INPUT:
            if (isServer)
            {
                SLNet::BitStream bsIn(packet->data, packet->length, false);
                bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
                int playerIndex;
                int x, y;
                bsIn.Read(playerIndex);
                bsIn.Read(x);
                bsIn.Read(y);
                players[playerIndex].setPosition(x, y);
            }
            break;
        case ID_GAME_STATE:
            if (!isServer)
            {
                SLNet::BitStream bsIn(packet->data, packet->length, false);
                bsIn.IgnoreBytes(sizeof(SLNet::MessageID));
                for (auto &player : players)
                {
                    int x, y;
                    Uint8 r, g, b;
                    bsIn.Read(x);
                    bsIn.Read(y);
                    bsIn.Read(r);
                    bsIn.Read(g);
                    bsIn.Read(b);
                    player.setPosition(x, y);
                    player.setColor({r, g, b});
                }
            }
            break;
        default:
            std::cout << "Received a message with identifier " << packet->data[0] << std::endl;
            break;
        }
    }

    if (!isServer && clientPlayerIndex != -1)
    {
        // Send player input to the server
        SLNet::BitStream bsOut;
        bsOut.Write((SLNet::MessageID)ID_PLAYER_INPUT);
        bsOut.Write(clientPlayerIndex); // Send the player index
        bsOut.Write(players[clientPlayerIndex].getX());
        bsOut.Write(players[clientPlayerIndex].getY());
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
    }
}
void Game::startServer(SLNet::RakPeerInterface *server)
{
    SLNet::SocketDescriptor sd(SERVER_PORT, 0);
    server->Startup(10, &sd, 1);
    server->SetMaximumIncomingConnections(10);

    std::cout << "Server started and listening on port " << SERVER_PORT << std::endl;
}

void Game::startClient(SLNet::RakPeerInterface *client)
{
    SLNet::SocketDescriptor sd;
    client->Startup(1, &sd, 1);
    client->Connect(SERVER_IP, SERVER_PORT, 0, 0);

    std::cout << "Client started and connecting to server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    // Generate random color for the client player
    Uint8 r = rand() % 256;
    Uint8 g = rand() % 256;
    Uint8 b = rand() % 256;

    // Add a player for the client
    players.push_back(Player(100, 100, {r, g, b}));
}