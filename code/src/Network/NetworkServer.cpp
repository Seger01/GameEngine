#include "NetworkServer.h"
#include "Network/NetworkInformation.h"

#include <iostream>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/PacketLogger.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>
#include <string>

NetworkServer::NetworkServer()
    : mServer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance),
      mLastBroadcastCheck(std::chrono::steady_clock::now()) {
    // std::cout << "Server Address: " << mServer->GetLocalIP(0) << std::endl;
    // SLNet::SocketDescriptor sd(SERVER_PORT, mServer->GetLocalIP(0));
    SLNet::SocketDescriptor sd(SERVER_PORT, 0);
    sd.socketFamily = AF_INET;
    SLNet::StartupResult result = mServer->Startup(10, &sd, 1);
    mServer->SetMaximumIncomingConnections(10);
    if (result != SLNet::RAKNET_STARTED) {
        throw std::runtime_error("Failed to start server");
    }
}

void NetworkServer::handleClientConnection() {
    throw std::runtime_error("NetworkServer::handleClientConnection() not implemented");
}

void NetworkServer::receiveGameState() {
    throw std::runtime_error("NetworkServer::receiveGameState() not implemented");
}

void NetworkServer::sendGameState() { throw std::runtime_error("NetworkServer::sendGameState() not implemented"); }

void NetworkServer::update(std::vector<GameObject*>& aGameObjects) {
    if (!mServer->IsActive()) {
        throw std::runtime_error("Server is not running");
    }
    SLNet::Packet* packet;
    for (packet = mServer->Receive(); packet; mServer->DeallocatePacket(packet), packet = mServer->Receive()) {
        switch (packet->data[0]) {
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
        // case ID_UNCONNECTED_PING: {
        //     std::cout << "Received ping from " << packet->systemAddress.ToString(true) << std::endl;
        //     SLNet::BitStream bs;
        //     bs.Write((SLNet::MessageID)ID_UNCONNECTED_PONG);
        //     mServer->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, packet->systemAddress, false);
        //     break;
        // }
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }
}