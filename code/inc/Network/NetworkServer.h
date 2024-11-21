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
    NetworkServer(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate);
    void receiveGameState();
    void sendGameState();
    void update(std::vector<std::reference_wrapper<GameObject>>& aGameObjects);

    bool isConnected() const;

    void spawnPlayerForNewClient(SLNet::RakNetGUID playerID);
    void sendPlayerInstantiation(SLNet::RakNetGUID playerID);

private:
    void handleIncomingPackets();
    void sendTransform();
    void handleTransform(SLNet::Packet* aPacket);
    void spawnNewPlayer(SLNet::Packet* aPacket);
    void onClientDisconnected(SLNet::RakNetGUID aClientID);
    void sendToAllClients(SLNet::BitStream& aBitStream);
    void sendPackets();
    void makeBitStream(SLNet::BitStream& aBitStream, SLNet::MessageID aMessageID);
    void getBitStreamData(SLNet::BitStream& aBitStream);
    void getBitStreamData(SLNet::BitStream& aBitStream, std::chrono::milliseconds::rep& aTimeStamp);
    void getBitStreamData(SLNet::BitStream& aBitStream, SLNet::RakNetGUID& aGUID);
    void getBitStreamData(SLNet::BitStream& aBitStream, std::chrono::milliseconds::rep& aTimeStamp,
                          SLNet::RakNetGUID& aGUID);
    void setBitStreamGUID(SLNet::BitStream& aBitStream, SLNet::RakNetGUID aGUID);

private:
    std::vector<std::reference_wrapper<GameObject>>& mObjects;

    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mServer;
    std::chrono::time_point<std::chrono::steady_clock> mLastSendPacketsTime;

    int mTickRate;
};

#endif // NETWORKSERVER_H