#include "game.h"
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;
#define SERVER_PORT 60000
#define SERVER_IP "127.0.0.1"

enum GameMessages
{
    ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};

Game::Game()
    : window(NULL), renderer(NULL), quit(false), frameDelay(1000 / 60), peer(NULL), isServer(false) {}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("SDL2 Multiplayer Block Control", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    // Initialize players
    players.push_back(Player(100, 100, {255, 0, 0})); // Red block
    players.push_back(Player(400, 300, {0, 0, 255})); // Blue block

    // Initialize SLikeNet
    peer = SLNet::RakPeerInterface::GetInstance();
    SLNet::SocketDescriptor sd;
    if (isServer)
    {
        Game::startServer(peer);
    }
    else
    {
        Game::startClient(peer);
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

    // Handle input for both players
    players[0].handleInput(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);
    players[1].handleInput(SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);
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
            std::cout << "A connection is incoming.\n";
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Our connection request has been accepted.\n";
            break;
        case ID_GAME_MESSAGE_1:
            std::cout << "Received a game message.\n";
            break;
        default:
            std::cout << "Received a message with identifier " << packet->data[0] << std::endl;
            break;
        }
    }

    // Send a game message to the server
    SLNet::BitStream bsOut;
    bsOut.Write((SLNet::MessageID)ID_GAME_MESSAGE_1);
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Game::startServer(SLNet::RakPeerInterface *server)
{
    SLNet::SocketDescriptor sd(SERVER_PORT, 0);
    server->Startup(10, &sd, 1);
    server->SetMaximumIncomingConnections(10);

    std::cout << "Server started and listening on port " << SERVER_PORT << std::endl;

    while (true)
    {
        for (SLNet::Packet *packet = server->Receive(); packet; server->DeallocatePacket(packet), packet = server->Receive())
        {
            switch (packet->data[0])
            {
            case ID_NEW_INCOMING_CONNECTION:
                std::cout << "A connection is incoming.\n";
                break;
            case ID_GAME_MESSAGE_1:
                std::cout << "Received a game message.\n";
                break;
            default:
                std::cout << "Received a message with identifier " << packet->data[0] << std::endl;
                break;
            }
        }
    }
}

void Game::startClient(SLNet::RakPeerInterface *client)
{
    SLNet::SocketDescriptor sd;
    client->Startup(1, &sd, 1);
    client->Connect(SERVER_IP, SERVER_PORT, 0, 0);

    std::cout << "Client started and connecting to server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    while (true)
    {
        for (SLNet::Packet *packet = client->Receive(); packet; client->DeallocatePacket(packet), packet = client->Receive())
        {
            switch (packet->data[0])
            {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "Our connection request has been accepted.\n";
                break;
            case ID_GAME_MESSAGE_1:
                std::cout << "Received a game message.\n";
                break;
            default:
                std::cout << "Received a message with identifier " << packet->data[0] << std::endl;
                break;
            }
        }

        // Send a game message to the server
        SLNet::BitStream bsOut;
        bsOut.Write((SLNet::MessageID)ID_GAME_MESSAGE_1);
        client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
    }
}