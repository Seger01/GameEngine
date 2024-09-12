#include "client.h"
#include "gameClient.h"

#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/sleep.h>

#define SERVER_PORT 60000
#define CLIENT_PORT 60001
#define SERVER_IP "127.0.0.1"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;

Client::Client()
{
}

void Client::init()
{
    std::srand(std::time(0));

    // Initialize SLikeNet
    peer = SLNet::RakPeerInterface::GetInstance();
    SLNet::SocketDescriptor sd(CLIENT_PORT, 0);
    peer->Startup(1, &sd, 1);

    // Try to connect to the server
    std::cout << "Attempting to connect to the server..." << std::endl;
    SLNet::ConnectionAttemptResult car = peer->Connect(SERVER_IP, SERVER_PORT, nullptr, 0);

    if (car != SLNet::CONNECTION_ATTEMPT_STARTED)
    {
        std::cerr << "Connection attempt failed! Error code: " << car << std::endl;
        return;
    }

    // Loop until a connection is established or failed
    bool isConnected = false;
    while (!isConnected)
    {
        for (SLNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
        {
            switch (packet->data[0])
            {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "Connection successful!" << std::endl;
                isConnected = true;
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cerr << "Connection attempt failed." << std::endl;
                isConnected = true; // Break the loop, since connection failed
                break;
            default:
                break;
            }
        }

        // Optional: sleep to prevent high CPU usage in the loop
        RakSleep(100);
    }

    // Generate random color for the client player
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    int x = rand() % (SCREEN_WIDTH - BLOCK_SIZE);
    int y = rand() % (SCREEN_HEIGHT - BLOCK_SIZE);

    PlayerInit playerInit;
    playerInit.colorRed = r;
    playerInit.colorGreen = g;
    playerInit.colorBlue = b;
    playerInit.posX = x;
    playerInit.posY = y;

    // Send the color and initial position to the server
    SLNet::BitStream bsOut;
    bsOut.Write((SLNet::MessageID)ID_PLAYER_INIT);
    bsOut.Write(playerInit);
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::run()
{
    // Start game
    GameClient game(peer);
    game.init();
    game.run();
    game.cleanup();
}

void Client::cleanup()
{
    SLNet::RakPeerInterface::DestroyInstance(peer);
}