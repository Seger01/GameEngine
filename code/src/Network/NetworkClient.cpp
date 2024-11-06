#include "NetworkClient.h"
#include <iostream>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>

NetworkClient::NetworkClient()
    : mPeer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance), mIsConnected(false) {
    connectToServer();
}

void NetworkClient::connectToServer() {
    if (mIsConnected || mIsConnecting) {
        return;
    }
    SLNet::SocketDescriptor sd(CLIENT_PORT, SERVER_ADDRESS);
    SLNet::ConnectionAttemptResult result = mPeer->Connect(SERVER_ADDRESS, SERVER_PORT, nullptr, 0);
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