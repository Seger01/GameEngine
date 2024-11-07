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
    void connectToServer();
    void sendGameState();
    void receiveGameState();
    void update(std::vector<GameObject*>& aGameObjects);
    void discoverServers();
    std::vector<std::string>& getServerAddresses();
    void setServerAddress(std::string aServerAddress);

private:
    bool mIsConnected;
    bool mIsConnecting;
    int mClientID;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mClient;
    std::vector<std::string> mServerAddresses;
    std::string mServerAddress;
};

#endif // NETWORKCLIENT_H