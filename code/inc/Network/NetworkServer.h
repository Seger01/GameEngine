/**
 * @file NetworkServer.h
 * @brief Header file for the NetworkServer class.
 */

#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "GameObject.h"
#include "Network/NetworkClient.h"

#include <chrono>
#include <list>
#include <memory>
#include <slikenet/peerinterface.h>
#include <slikenet/types.h>
#include <vector>

class NetworkObject;
/**
 * @class NetworkServer
 * @brief Manages the server-side networking for the game.
 */
class NetworkServer {
friend class NetworkManager;
public:
	NetworkServer(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate);
	void update();
	bool isConnected() const;

private:
    void handleIncomingPackets();
	void sendPackets();

	void sendTransform();
	void sendCustomSerialize();
	void sendPlayerInit(SLNet::RakNetGUID playerID);
	void sendPrefabSpawn(GameObject& aObject, SLNet::RakNetGUID clientID = SLNet::UNASSIGNED_RAKNET_GUID);
	void sendPrefabDespawn(NetworkObject& aObject);

	void handleTransform(SLNet::Packet* aPacket);
	void handleCustomSerialize(SLNet::Packet* aPacket);
	void handlePlayerInit(SLNet::Packet* aPacket);
	void handleClientDisconnect(SLNet::RakNetGUID aClientID);

	void sendToAllClients(SLNet::BitStream& aBitStream);
	void sendToClient(SLNet::BitStream& aBitStream, SLNet::RakNetGUID clientID);
	void spawnObjectsForNewClient(SLNet::RakNetGUID playerID);

private:
	/**
	 * @brief Reference to the game objects managed by the server.
	 */
	std::vector<std::reference_wrapper<GameObject>>& mObjects;

	/**
	 * @brief Unique pointer to the RakNet peer interface for the server.
	 */
	std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mServer;

	/**
	 * @brief Time point of the last packet sent.
	 */
	std::chrono::time_point<std::chrono::steady_clock> mLastSendPacketsTime;

	/**
	 * @brief Tick rate for the server updates.
	 */
	int mTickRate;
};

#endif // NETWORKSERVER_H