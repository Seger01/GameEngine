#include "server.h"
#include "gameServer.h"

#include <iostream>
#include <chrono>

#include <slikenet/sleep.h>

#define SERVER_PORT 60000
#define SERVER_IP "192.168.2.100"
#define TIMEOUT_SECONDS 5

Server::Server()
{
}

void Server::init()
{ // Initialize SLikeNet
    peer = SLNet::RakPeerInterface::GetInstance();

    SLNet::SocketDescriptor sd(SERVER_PORT, 0);
    peer->Startup(10, &sd, 1);
    peer->SetMaximumIncomingConnections(10);
    // Check if the server has started
    auto start = std::chrono::steady_clock::now();
    bool serverStarted = false;

    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(TIMEOUT_SECONDS))
    {
        if (peer->IsActive())
        {
            serverStarted = true;
            break;
        }
        RakSleep(100);
    }

    if (!serverStarted)
    {
        std::cerr << "Server failed to start within " << TIMEOUT_SECONDS << " seconds." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started with ip address: " << SERVER_IP << " and listening on port: " << SERVER_PORT << std::endl;
}

void Server::run()
{
    // Start game
    GameServer game(peer);
    game.init();
    game.run();
    game.cleanup();
}

void Server::cleanup()
{
    SLNet::RakPeerInterface::DestroyInstance(peer);
}