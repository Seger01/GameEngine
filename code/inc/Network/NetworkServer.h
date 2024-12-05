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
	void update();
	bool isConnected() const;

private:
    void handleIncomingPackets();
	void sendPackets();

	void sendTransform();
	void sendCustomSerialize();
	void spawnPlayerForNewClient(SLNet::RakNetGUID playerID);
	void sendPlayerInstantiation(SLNet::RakNetGUID playerID);
	void spawnNewPlayer(SLNet::Packet* aPacket);

	void onClientDisconnected(SLNet::RakNetGUID aClientID);
	void handleTransform(SLNet::Packet* aPacket);
	void handleCustomSerialize(SLNet::Packet* aPacket);

	void sendToAllClients(SLNet::BitStream& aBitStream);

private:
    std::vector<std::reference_wrapper<GameObject>>& mObjects;

    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mServer;
    std::chrono::time_point<std::chrono::steady_clock> mLastSendPacketsTime;

    int mTickRate;
};

#endif // NETWORKSERVER_H