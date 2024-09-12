#include "gameServer.h"
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

GameServer::GameServer(SLNet::RakPeerInterface *peer)
    : GameBase(peer), nextClientId(0), clientIds({})
{
}

void GameServer::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
    }
}

void GameServer::handleNetwork()
{
    for (SLNet::Packet *packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_NEW_INCOMING_CONNECTION:
        {

            std::cout << "A connection is incoming.\n";
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

            break;
        }
        case ID_PLAYER_INIT:
        {
            std::cout << "Received a message with identifier ID_PLAYER_INIT" << std::endl;
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
            break;
        }
        default:
            std::cout << "Received a message with identifier " << "Default" << std::endl;
            break;
        }
    }
}