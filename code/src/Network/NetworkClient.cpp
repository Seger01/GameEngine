#include "NetworkClient.h"
#include <iostream>
#include <regex>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>

NetworkClient::NetworkClient(unsigned short aClientPort, unsigned short aServerPort, unsigned short aBroadcastPort)
    : mPeer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance), mIsConnected(false),
      mIsConnecting(false), mClientID(-1), SERVER_PORT(aServerPort), CLIENT_PORT(aClientPort),
      BROADCAST_PORT(aBroadcastPort), SERVER_ADDRESS("0.0.0.0") {}

void NetworkClient::connectToServer() {
    if (mIsConnected || mIsConnecting) {
        return;
    }
    if (SERVER_ADDRESS == "0.0.0.0") {
        throw std::runtime_error("Server address not set");
    }
    SLNet::SocketDescriptor sd(CLIENT_PORT, SERVER_ADDRESS.c_str());
    SLNet::ConnectionAttemptResult result = mPeer->Connect(SERVER_ADDRESS.c_str(), SERVER_PORT, nullptr, 0);
    if (result != SLNet::CONNECTION_ATTEMPT_STARTED) {
        throw std::runtime_error("Failed to start connection attempt");
    }
}

void NetworkClient::sendGameState() { throw std::runtime_error("NetworkClient::sendGameState() not implemented"); }

void NetworkClient::receiveGameState() {
    throw std::runtime_error("NetworkClient::receiveGameState() not implemented");
}

void NetworkClient::update(std::vector<GameObject*>& aGameObjects) {
    SLNet::Packet* packet;
    for (packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive()) {
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
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }

    if (!mIsConnected && !mIsConnecting) {
        connectToServer();
    }
}

void NetworkClient::discoverServers(int aTimeoutMs) {
    int mBroadcastSocket = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in broadcastAddr;
    socklen_t addrLen = sizeof(broadcastAddr);

    if (mBroadcastSocket < 0) {
        throw std::runtime_error("Failed to create UDP socket");
    }

    int broadcastEnable = 1;
    setsockopt(mBroadcastSocket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255"); // Broadcast address
    broadcastAddr.sin_port = htons(60000);                        // Discovery port

    const char* message = "DISCOVER_SERVER";
    sendto(mBroadcastSocket, message, strlen(message), 0, (struct sockaddr*)&broadcastAddr, addrLen);

    mStartServerDiscovery = std::chrono::steady_clock::now();
    mDiscoveryTimeoutMs = aTimeoutMs;
}

bool NetworkClient::getServerAddresses(std::vector<std::string>& aServerAddresses) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - mStartServerDiscovery)
            .count() >= mDiscoveryTimeoutMs) {
        return false;
    }

    struct sockaddr_in broadcastAddr;
    socklen_t addrLen = sizeof(broadcastAddr);

    char buffer[1024];
    int bytesReceived;
    while (bytesReceived =
               recvfrom(mBroadcastSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&broadcastAddr, &addrLen)) {
        if (bytesReceived < 0) {
            break;
        }
        buffer[bytesReceived] = '\0';
        std::cout << "Discovered server at: " << buffer << std::endl;
        aServerAddresses.push_back(buffer);
    }

    close(mBroadcastSocket);

    return true;
}

void NetworkClient::setServerAddress(std::string aServerAddress) {
    const std::regex ipPattern(
        R"(\b((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\b)");
    if (!std::regex_match(aServerAddress, ipPattern)) {
        throw std::runtime_error("Invalid IP address");
    }

    SERVER_ADDRESS = aServerAddress;
}