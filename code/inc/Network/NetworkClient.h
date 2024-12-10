/**
 * @file NetworkClient.h
 * @brief Header file for the NetworkClient class.
 */

#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "GameObject.h"

#include <slikenet/peerinterface.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

/**
 * @class NetworkClient
 * @brief Manages network client operations including connecting to server, sending and receiving packets.
 */
class NetworkClient {
public:
    NetworkClient(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate);
    ~NetworkClient();

    void connectToServer();
    void update();
    void discoverServers();
    std::vector<std::string>& getServerAddresses();
    void setServerAddress(std::string aServerAddress);
    bool isConnected() const;

private:
    void sendPackets();
    void sendToServer(SLNet::BitStream& aBitStream);
    void sendTransform();
    void sendPlayerInit();
    void sendCustomSerialize();

    void handleIncomingPackets();
    void handleTransform(SLNet::Packet* aPacket);
    void handlePlayerInstantiation(SLNet::Packet* aPacket);
    void handleCustomSerialize(SLNet::Packet* aPacket);
    void handlePlayerDestruction(SLNet::Packet* aPacket);

private:
    bool mIsConnected; ///< Indicates if the client is connected to the server.
    bool mIsConnecting; ///< Indicates if the client is in the process of connecting to the server.
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mClient; ///< RakNet client interface.
    std::vector<std::string> mServerAddresses; ///< List of discovered server addresses.
    std::string mServerAddress; ///< Server address to connect to.
    SLNet::RakNetGUID mServerGUID; ///< GUID of the connected server.

    std::vector<std::reference_wrapper<GameObject>>& mObjects; ///< Reference to a vector of GameObjects.
    int mTickRate; ///< Tick rate for sending packets.
    std::chrono::time_point<std::chrono::steady_clock> mLastSendPacketsTime; ///< Last time packets were sent.
};

#endif // NETWORKCLIENT_H