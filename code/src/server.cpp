#include "server.h"
#include "gameServer.h"

#include <iostream>

#define SERVER_PORT 60000
#define SERVER_IP "127.0.0.1"

Server::Server()
{
}

void Server::init()
{ // Initialize SLikeNet
    peer = SLNet::RakPeerInterface::GetInstance();

    SLNet::SocketDescriptor sd(SERVER_PORT, 0);
    peer->Startup(10, &sd, 1);
    peer->SetMaximumIncomingConnections(10);

    std::cout << "Server started and listening on port " << SERVER_PORT << std::endl;
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