#include "NetworkClient.h"
#include "Network/NetworkInformation.h"
#include <iostream>
#include <regex>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>

NetworkClient::NetworkClient()
    : mClient(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance), mIsConnected(false),
      mIsConnecting(false), mClientID(-1), mServerAddress("0.0.0.0"), mGameObjects(nullptr) {
    // std::cout << "Client Address: " << mClient->GetLocalIP(0) << std::endl;
    // SLNet::SocketDescriptor sd(CLIENT_PORT, mClient->GetLocalIP(0));
    SLNet::SocketDescriptor sd(CLIENT_PORT, 0);
    sd.socketFamily = AF_INET;
    SLNet::StartupResult result = mClient->Startup(1, &sd, 1);
    if (result != SLNet::RAKNET_STARTED) {
        throw std::runtime_error("Failed to start client");
    }
}

void NetworkClient::connectToServer() {
    if (mIsConnected || mIsConnecting) {
        return;
    }
    if (mServerAddress == "0.0.0.0") {
        throw std::runtime_error("Server address not set");
    }
    SLNet::ConnectionAttemptResult result = mClient->Connect(mServerAddress.c_str(), SERVER_PORT, nullptr, 0);
    if (result != SLNet::CONNECTION_ATTEMPT_STARTED) {
        throw std::runtime_error("Failed to start connection attempt");
    }
}

void NetworkClient::sendGameState() { throw std::runtime_error("NetworkClient::sendGameState() not implemented"); }

void NetworkClient::receiveGameState() {
    throw std::runtime_error("NetworkClient::receiveGameState() not implemented");
}

void NetworkClient::update(std::vector<GameObject*>& aGameObjects) {
    mGameObjects = &aGameObjects;
    if (!mClient->IsActive()) {
        throw std::runtime_error("Client is not running");
    }
    handleIncomingPackets();
}

void NetworkClient::discoverServers() {
    mClient->Ping("255.255.255.255", SERVER_PORT, false); // Send discovery ping to the broadcast address
    mServerAddresses.clear();
}

std::vector<std::string>& NetworkClient::getServerAddresses() { return mServerAddresses; }

void NetworkClient::setServerAddress(std::string aServerAddress) {
    const std::regex ipPattern(
        R"(\b((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\b)");
    if (!std::regex_match(aServerAddress, ipPattern)) {
        throw std::runtime_error("Invalid IP address");
    }

    mServerAddress = aServerAddress;
}

void NetworkClient::handleIncomingPackets() {
    SLNet::Packet* packet;
    for (packet = mClient->Receive(); packet; mClient->DeallocatePacket(packet), packet = mClient->Receive()) {
        SLNet::BitStream bs(packet->data, packet->length, false);
        switch (packet->data[0]) {
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Connected to server.\n";
            mIsConnected = true;
            mIsConnecting = false;
            break;
        case ID_CONNECTION_ATTEMPT_FAILED:
            std::cout << "Connection attempt failed. Retrying...\n";
            mIsConnecting = false;
            mIsConnected = false;
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
        case ID_DISCONNECTION_NOTIFICATION:
        case ID_CONNECTION_LOST:
            std::cout << "Connection lost or disconnected.\n";
            mIsConnected = false;
            break;
        case ID_UNCONNECTED_PONG: {
            // Handle the pong response from the server
            bs.IgnoreBytes(sizeof(SLNet::MessageID));
            std::string serverIp;
            serverIp = packet->systemAddress.ToString(false);
            std::cout << "Discovered server at: " << serverIp << std::endl;
            mServerAddresses.push_back(serverIp);

            setServerAddress(serverIp);
            connectToServer();
            break;
        }
        case ID_UNCONNECTED_PING: {
            std::cout << "Got ping from " << packet->systemAddress.ToString() << std::endl;
        }
        case ID_UNCONNECTED_PING_OPEN_CONNECTIONS: {
            std::cout << "Got open connection ping from " << packet->systemAddress.ToString() << std::endl;
        }
        case NetworkMessage::ID_TRANSFORM_PACKET:
            handleTransform(packet);
            break;
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }
}

void NetworkClient::sendTransform() {
    throw std::runtime_error("NetworkClient::sendTransform() not implemented");
    // SLNet::BitStream bs;
    // bs.Write((SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET);
    // for (auto& gameObject : *mGameObjects) {
    //     bs.Write(gameObject->mPosition.x);
    //     bs.Write(gameObject->mPosition.y);
    //     bs.Write(gameObject->mPosition.z);
    // }
    // mClient->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkClient::handleTransform(SLNet::Packet* aPacket) {
    throw std::runtime_error("NetworkClient::handleTransform() not implemented");
    // SLNet::BitStream bs(aPacket->data, aPacket->length, false);
    // bs.IgnoreBytes(sizeof(SLNet::MessageID));
    // for (auto& gameObject : *mGameObjects) {
    //     bs.Read(gameObject->mPosition.x);
    //     bs.Read(gameObject->mPosition.y);
    //     bs.Read(gameObject->mPosition.z);
    // }
}