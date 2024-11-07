#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "GameObject.h"
#include "NetworkClient.h"
#include <chrono>
#include <ifaddrs.h>
#include <list>
#include <memory>
#include <slikenet/peerinterface.h>
#include <vector>

class NetworkServer {
public:
    NetworkServer(unsigned short aServerPort, unsigned short aBroadcastPort);
    void handleClientConnection();
    void receiveGameState();
    void sendGameState();
    void update(std::vector<GameObject*>& aGameObjects);

private:
    void getLocalIPAddress();
    void setupBroadcastListener();
    void listenForBroadcasts();

private:
    std::list<NetworkClient> mConnectedClients;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mPeer;
    std::string mServerAddress;
    int mBroadcastSocket;
    std::chrono::time_point<std::chrono::steady_clock> mLastBroadcastCheck;
    unsigned short SERVER_PORT;
    unsigned short BROADCAST_PORT;
};

#endif // NETWORKSERVER_H