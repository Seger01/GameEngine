#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "NetworkClient.h"

#include "GameObject.h"

#include <chrono>
#include <list>
#include <memory>
#include <slikenet/peerinterface.h>
#include <slikenet/types.h>
#include <vector>

class NetworkServer {
public:
    NetworkServer(int aTickRate);
    void update(std::vector<GameObject*>& aGameObjects);

    bool isConnected() const;

    void spawnPlayerForNewClient(SLNet::RakNetGUID playerID);
    void sendPlayerInstantiation(SLNet::RakNetGUID playerID);

private:
    void handleIncomingPackets();
    void sendTransform();
    void handleTransform(SLNet::Packet* aPacket);
    void handleCustomSerialize(SLNet::Packet* aPacket);
    void spawnNewPlayer(SLNet::Packet* aPacket);
    void onClientDisconnected(SLNet::RakNetGUID aClientID);
    void sendToAllClients(SLNet::BitStream& aBitStream);
    void sendPackets();
    void sendCustomSerialize();

private:
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mServer;
    std::chrono::time_point<std::chrono::steady_clock> mLastSendPacketsTime;

    std::vector<GameObject*>* mGameObjects;
    int mTickRate;
};

#endif // NETWORKSERVER_H