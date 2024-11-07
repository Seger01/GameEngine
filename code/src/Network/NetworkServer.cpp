#include "NetworkServer.h"
#include <arpa/inet.h>
#include <iostream>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/PacketLogger.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>
#include <string>

NetworkServer::NetworkServer(unsigned short aServerPort, unsigned short aBroadcastPort)
    : mPeer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance), SERVER_PORT(aServerPort),
      BROADCAST_PORT(aBroadcastPort), mLastBroadcastCheck(std::chrono::steady_clock::now()) {
    getLocalIPAddress();
    std::cout << "Server IP address: " << mServerAddress << std::endl;
    if (mServerAddress.empty()) {
        throw std::runtime_error("Failed to get local IP address");
    }
    SLNet::SocketDescriptor sd(SERVER_PORT, mServerAddress.c_str());
    SLNet::StartupResult result = mPeer->Startup(10, &sd, 1);
    if (result != SLNet::RAKNET_STARTED) {
        throw std::runtime_error("Failed to start server");
    }
    mPeer->SetMaximumIncomingConnections(10);

    setupBroadcastListener();
}

void NetworkServer::handleClientConnection() {
    throw std::runtime_error("NetworkServer::handleClientConnection() not implemented");
}

void NetworkServer::receiveGameState() {
    throw std::runtime_error("NetworkServer::receiveGameState() not implemented");
}

void NetworkServer::sendGameState() { throw std::runtime_error("NetworkServer::sendGameState() not implemented"); }

void NetworkServer::update(std::vector<GameObject*>& aGameObjects) {
    SLNet::Packet* packet;
    for (packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive()) {
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
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - mLastBroadcastCheck).count() >= 1) {
        listenForBroadcasts();
        mLastBroadcastCheck = now;
    }
}

void NetworkServer::getLocalIPAddress() {
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in* sa;
    std::string addr;

    // Get the list of network interfaces
    if (getifaddrs(&ifap) == 0) {
        // Iterate over each interface
        for (ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next) {
            // Check if the interface has an IPv4 address
            if (ifa->ifa_addr->sa_family == AF_INET) {
                sa = (struct sockaddr_in*)ifa->ifa_addr;
                addr = inet_ntoa(sa->sin_addr);
                // Skip the loopback interface
                if (std::string(ifa->ifa_name) != "lo") {
                    freeifaddrs(ifap);
                    mServerAddress = addr;
                    return; // Exit the function after finding a valid IP address
                }
            }
        }
        freeifaddrs(ifap);
    }
    // Throw an error if no valid IP address is found
    throw std::runtime_error("Failed to get local IP address");
}

void NetworkServer::setupBroadcastListener() {
    mBroadcastSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mBroadcastSocket < 0) {
        throw std::runtime_error("Failed to create UDP socket");
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces
    serverAddr.sin_port = htons(60000);             // Discovery port

    if (bind(mBroadcastSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(mBroadcastSocket);
        throw std::runtime_error("Failed to bind UDP socket");
    }

    // Set socket to non-blocking mode
    fcntl(mBroadcastSocket, F_SETFL, O_NONBLOCK);
}

void NetworkServer::listenForBroadcasts() {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024];

    int bytesReceived =
        recvfrom(mBroadcastSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (bytesReceived > 0) {
        // Respond to discovery request
        const char* replyMessage = mServerAddress.c_str();
        sendto(mBroadcastSocket, replyMessage, strlen(replyMessage), 0, (struct sockaddr*)&clientAddr, clientAddrLen);
    }
}