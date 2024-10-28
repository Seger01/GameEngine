#include "NetworkServer.h"
#include <stdexcept>
#include <slikenet/peerinterface.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <slikenet/PacketLogger.h>
#include <iostream>

NetworkServer::NetworkServer()
    : mPeer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance)
{
    SLNet::SocketDescriptor sd(SERVER_PORT, SERVER_ADDRESS);
    SLNet::StartupResult result = mPeer->Startup(10, &sd, 1);
    if (result != SLNet::RAKNET_STARTED)
    {
        throw std::runtime_error("Failed to start server");
    }
    mPeer->SetMaximumIncomingConnections(10);
}

// Other methods remain unchanged
void NetworkServer::handleClientConnection()
{
    throw std::runtime_error("NetworkServer::handleClientConnection() not implemented");
}

void NetworkServer::receiveGameState()
{
    throw std::runtime_error("NetworkServer::receiveGameState() not implemented");
}

void NetworkServer::sendGameState()
{
    throw std::runtime_error("NetworkServer::sendGameState() not implemented");
}

void NetworkServer::update()
{
    SLNet::Packet *packet;
    for (packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_NEW_INCOMING_CONNECTION:
            std::cout << "A connection is incoming.\n";
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Our connection request has been accepted.\n";
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            std::cout << "The server is full.\n";
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            std::cout << "A client has disconnected.\n";
            break;
        case ID_CONNECTION_LOST:
            std::cout << "A client lost the connection.\n";
            break;
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }
}