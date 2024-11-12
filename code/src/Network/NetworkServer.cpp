#include "NetworkServer.h"

#include "Network/NetworkInformation.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkTransform.h"

#include "Engine/EngineBravo.h"

#include <iostream>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/PacketLogger.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>
#include <string>

NetworkServer::NetworkServer(int aTickRate = 60)
    : mServer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance),
      mLastSendPacketsTime(std::chrono::steady_clock::now()), mGameObjects(nullptr), mTickRate(aTickRate) {
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

void NetworkServer::receiveGameState() {
    throw std::runtime_error("NetworkServer::receiveGameState() not implemented");
}

void NetworkServer::sendGameState() { throw std::runtime_error("NetworkServer::sendGameState() not implemented"); }

void NetworkServer::update(std::vector<GameObject*>& aGameObjects) {
    mGameObjects = &aGameObjects;
    if (!mServer->IsActive()) {
        throw std::runtime_error("Server is not running");
    }
    handleIncomingPackets();
    sendPackets();
}

bool NetworkServer::isConnected() const { return mServer->IsActive(); }

void NetworkServer::sendPlayerInstantiation(SLNet::RakNetGUID playerID) {
    std::cout << "Sending player instantiation message to all clients.\n";
    SLNet::BitStream bs;
    bs.Write((SLNet::MessageID)ID_PLAYER_INIT);
    bs.Write(playerID);

    sendToAllClients(bs);
}

void NetworkServer::handleIncomingPackets() {
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
            onClientDisconnected(packet->guid);
            break;
        case ID_CONNECTION_LOST:
            std::cout << "A client lost the connection.\n";
            break;
        case (SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET:
            std::cout << "Received transform packet.\n";
            handleTransform(packet);
            break;
        case (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT:
            std::cout << "Received player instantiation message.\n";
            spawnNewPlayer(packet);
            break;
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }
}

void NetworkServer::sendTransform() {
    if (!mGameObjects) {
        throw std::runtime_error("Game objects not set");
    }

    for (auto gameObject : *mGameObjects) {
        if (!gameObject->hasComponent<NetworkTransform>()) {
            continue;
        }

        Transform transform = gameObject->getTransform();
        NetworkTransform* networkTransform = gameObject->getComponents<NetworkTransform>()[0];
        SLNet::BitStream bs;
        bs.Write((SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET);

        if (networkTransform->getSendPositionX()) {
            bs.Write(transform.position.x);
        }
        if (networkTransform->getSendPositionY()) {
            bs.Write(transform.position.y);
        }
        if (networkTransform->getSendRotation()) {
            bs.Write(transform.rotation);
        }
        if (networkTransform->getSendScaleX()) {
            bs.Write(transform.scale.x);
        }
        if (networkTransform->getSendScaleY()) {
            bs.Write(transform.scale.y);
        }

        sendToAllClients(bs);
    }
}

void NetworkServer::handleTransform(SLNet::Packet* aPacket) {
    if (!mGameObjects) {
        throw std::runtime_error("Game objects not set");
    }
    SLNet::BitStream bs(aPacket->data, aPacket->length, false);
    bs.IgnoreBytes(sizeof(SLNet::MessageID));

    for (auto gameObject : *mGameObjects) {
        NetworkObject* networkObject = gameObject->getComponents<NetworkObject>()[0];
        if (networkObject->getClientID() != aPacket->guid) {
            continue;
        }
        if (gameObject->hasComponent<NetworkTransform>()) {
            Transform transform = gameObject->getTransform();
            NetworkTransform* networkTransform = gameObject->getComponents<NetworkTransform>()[0];
            if (networkTransform->getSendPositionX()) {
                bs.Read(transform.position.x);
            }
            if (networkTransform->getSendPositionY()) {
                bs.Read(transform.position.y);
            }
            if (networkTransform->getSendRotation()) {
                bs.Read(transform.rotation);
            }
            if (networkTransform->getSendScaleX()) {
                bs.Read(transform.scale.x);
            }
            if (networkTransform->getSendScaleY()) {
                bs.Read(transform.scale.y);
            }
            gameObject->setTransform(transform);
        }
    }
}

void NetworkServer::spawnNewPlayer(SLNet::Packet* aPacket) {
    SLNet::RakNetGUID clientID = aPacket->guid;
    // Instantiate player on the server for this client
    EngineBravo::getInstance().getNetworkManager().instantiatePlayer(clientID); // Server-side
                                                                                // instantiation
    sendPlayerInstantiation(clientID);
}

void NetworkServer::onClientDisconnected(SLNet::RakNetGUID clientID) {
    std::cout << "TODO destroy player object for client " << clientID.ToString() << std::endl;
}

void NetworkServer::sendToAllClients(SLNet::BitStream& aBitStream) {
    DataStructures::List<SLNet::SystemAddress> addresses;
    DataStructures::List<SLNet::RakNetGUID> guids;
    mServer->GetSystemList(addresses, guids);
    for (int i = 0; i < addresses.Size(); i++) {
        mServer->Send(&aBitStream, PacketPriority::MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
    }
}

void NetworkServer::sendPackets() {
    auto now = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::milliseconds(1000 / mTickRate);
    if (now - mLastSendPacketsTime < frameTime) {
        return;
    }
    sendTransform();
    mLastSendPacketsTime = std::chrono::steady_clock::now();
}