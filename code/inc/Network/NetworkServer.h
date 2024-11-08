#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "GameObject.h"
#include "NetworkClient.h"
#include <chrono>
#include <list>
#include <memory>
#include <slikenet/peerinterface.h>
#include <vector>

class NetworkServer {
public:
    NetworkServer();
    void handleClientConnection();
    void receiveGameState();
    void sendGameState();
    void update(std::vector<GameObject*>& aGameObjects);

private:
    void handleIncomingPackets();
    void sendTransform();
    void handleTransform(SLNet::Packet* aPacket);

private:
    std::list<std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)>> mConnectedClients;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mServer;
    std::string mServerAddress;
    int mBroadcastSocket;
    std::chrono::time_point<std::chrono::steady_clock> mLastBroadcastCheck;

    std::vector<GameObject*>* mGameObjects;
};

#endif // NETWORKSERVER_H