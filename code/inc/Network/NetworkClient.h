#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "GameObject.h"
#include <chrono>
#include <memory>
#include <slikenet/peerinterface.h>
#include <vector>

class NetworkClient {
public:
    NetworkClient(unsigned short aClientPort, unsigned short aServerPort, unsigned short aBroadcastPort);
    void connectToServer();
    void sendGameState();
    void receiveGameState();
    void update(std::vector<GameObject*>& aGameObjects);
    void discoverServers(int aTimeoutMs);
    bool getServerAddresses(std::vector<std::string>& aServerAddresses);
    void setServerAddress(std::string aServerAddress);

private:
    bool mIsConnected;
    bool mIsConnecting;
    int mClientID;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mPeer;
    std::string SERVER_ADDRESS;
    unsigned short SERVER_PORT;
    unsigned short CLIENT_PORT;
    unsigned short BROADCAST_PORT;

    int mBroadcastSocket;
    std::chrono::time_point<std::chrono::steady_clock> mStartServerDiscovery;
    int mDiscoveryTimeoutMs;
};

#endif // NETWORKCLIENT_H