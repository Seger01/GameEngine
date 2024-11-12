#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "GameObject.h"
#include <chrono>
#include <memory>
#include <slikenet/peerinterface.h>
#include <vector>

class NetworkClient {
public:
    NetworkClient();
    ~NetworkClient();
    void connectToServer();
    void sendGameState();
    void receiveGameState();
    void update(std::vector<GameObject*>& aGameObjects);
    void discoverServers();
    std::vector<std::string>& getServerAddresses();
    void setServerAddress(std::string aServerAddress);
    bool isConnected() const;

private:
    void sendPackets();
    void requestPlayerInstantiation();
    void handleIncomingPackets();
    void sendTransform();
    void handleTransform(SLNet::Packet* aPacket);
    void handlePlayerInstantiation(SLNet::Packet* aPacket);
    void handlePlayerID(SLNet::Packet* aPacket);

private:
    bool mIsConnected;
    bool mIsConnecting;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mClient;
    std::vector<std::string> mServerAddresses;
    std::string mServerAddress;

    std::vector<GameObject*>* mGameObjects;
};

#endif // NETWORKCLIENT_H