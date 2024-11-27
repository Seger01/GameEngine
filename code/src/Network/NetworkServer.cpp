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

NetworkServer::NetworkServer(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate = 60)
    : mServer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance),
      mLastSendPacketsTime(std::chrono::steady_clock::now()), mObjects(aObjects), mTickRate(aTickRate) {
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

void NetworkServer::update(std::vector<std::reference_wrapper<GameObject>>& aGameObjects) {
    mObjects = aGameObjects;
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
    makeBitStream(bs, (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT);
    setBitStreamGUID(bs, playerID);

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
    for (GameObject& gameObject : mObjects) {
        if (!gameObject.hasComponent<NetworkTransform>()) {
            continue;
        }

        Transform transform = gameObject.getTransform();
        NetworkTransform* networkTransform = gameObject.getComponents<NetworkTransform>()[0];
        SLNet::BitStream bs;
        makeBitStream(bs, (SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET);
        NetworkObject* networkObject = gameObject.getComponents<NetworkObject>()[0];
        setBitStreamGUID(bs, networkObject->getClientID());

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
    SLNet::BitStream bs(aPacket->data, aPacket->length, false);
    getBitStreamData(bs);

    for (GameObject& gameObject : mObjects) {
        if (!gameObject.hasComponent<NetworkObject>()) {
            continue;
        }
        NetworkObject* networkObject = gameObject.getComponents<NetworkObject>()[0];
        if (networkObject->getClientID() != aPacket->guid) {
            continue;
        }
        if (gameObject.hasComponent<NetworkTransform>()) {
            Transform transform = gameObject.getTransform();
            NetworkTransform* networkTransform = gameObject.getComponents<NetworkTransform>()[0];
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
            gameObject.setTransform(transform);
        }
    }
}

void NetworkServer::spawnNewPlayer(SLNet::Packet* aPacket) {
    SLNet::RakNetGUID clientID = aPacket->guid;
    // Instantiate player on the server for this client
    std::vector<GameObject*> persistantObjects =
        EngineBravo::getInstance().getSceneManager().getCurrentScene()->getPersistentGameObjects();
    for (auto object : persistantObjects) {
        if (object->hasComponent<NetworkObject>()) {
            NetworkObject* networkObject = object->getComponents<NetworkObject>()[0];
            if (networkObject->isPlayer() && !(networkObject->getClientID() == clientID)) {
                sendPlayerInstantiation(networkObject->getClientID());
            }
        }
    }
    EngineBravo::getInstance().getNetworkManager().instantiatePlayer(clientID); // Server-side
                                                                                // instantiation
    sendPlayerInstantiation(clientID);
}

void NetworkServer::onClientDisconnected(SLNet::RakNetGUID clientID) {
    EngineBravo::getInstance().getNetworkManager().destroyPlayer(clientID);

    SLNet::BitStream bs;
    makeBitStream(bs, (SLNet::MessageID)NetworkMessage::ID_PLAYER_DESTROY);
    setBitStreamGUID(bs, clientID);
    sendToAllClients(bs);
}

void NetworkServer::sendToAllClients(SLNet::BitStream& aBitStream) {
    DataStructures::List<SLNet::SystemAddress> addresses;
    DataStructures::List<SLNet::RakNetGUID> guids;
    mServer->GetSystemList(addresses, guids);
    for (int i = 0; i < addresses.Size(); i++) {
        mServer->Send(&aBitStream, PacketPriority::MEDIUM_PRIORITY, PacketReliability::RELIABLE, 0, guids[i], false);
    }
}

void NetworkServer::sendPackets() {
    auto now = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::milliseconds(1000 / mTickRate);
    if (now - mLastSendPacketsTime < frameTime) {
        return;
    }
    sendTransform();
    mLastSendPacketsTime = now;
}

void NetworkServer::makeBitStream(SLNet::BitStream& aBitStream, SLNet::MessageID aMessageID) {
    aBitStream.Reset();
    aBitStream.Write(aMessageID);

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    auto timestamp = duration.count();

    aBitStream.Write(timestamp);

    SLNet::RakNetGUID placeholderGUID = SLNet::UNASSIGNED_RAKNET_GUID;
    aBitStream.Write(placeholderGUID);
}

void NetworkServer::getBitStreamData(SLNet::BitStream& aBitStream) {
    SLNet::MessageID messageID;
    aBitStream.IgnoreBytes(sizeof(SLNet::MessageID));
    aBitStream.IgnoreBytes(sizeof(std::chrono::milliseconds::rep));
}

void NetworkServer::getBitStreamData(SLNet::BitStream& aBitStream, std::chrono::milliseconds::rep& aTimeStamp) {
    SLNet::MessageID messageID;
    aBitStream.IgnoreBytes(sizeof(SLNet::MessageID));
    aBitStream.Read(aTimeStamp);
}

void NetworkServer::setBitStreamGUID(SLNet::BitStream& aBitStream, SLNet::RakNetGUID aGUID) {
    // Find the position of the GUID and time in the bitstream
    size_t Position = sizeof(SLNet::MessageID) + sizeof(std::chrono::milliseconds::rep);
    SLNet::BitSize_t aWriteOffset = aBitStream.GetWriteOffset();
    aBitStream.SetWriteOffset(Position * 8); // Convert to bits
    aBitStream.Write(aGUID);
    aBitStream.SetWriteOffset(aWriteOffset); // Reset write offset
}
