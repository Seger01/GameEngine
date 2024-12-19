/**
 * @file NetworkManager.h
 * @brief Header file for the NetworkManager class.
 */
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "GameObject.h"
#include "Network/NetworkClient.h"
#include "Network/NetworkHost.h"
#include "Network/NetworkInformation.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkRegister.h"
#include "Network/NetworkServer.h"

#include <memory>

/**
 * @class Network
 * @brief Manages network operations including server, client, and host.
 */
class NetworkManager
{
	friend class NetworkClient;
	friend class NetworkServer;
	friend class UpdateQueue;

public:
	NetworkManager();

	void startNetwork();
	void shutdown();

	void initialize();
	void update();

	NetworkServer& getServer() const;
	NetworkClient& getClient() const;
	NetworkHost& getHost() const;

	bool isServer() const;
	bool isClient() const;
	bool isHost() const;
	bool isNetworked() const;
	bool isConnected() const;

	void setTickRate(int aTickRate);
	int getTickRate() const;

	void setDefaultPlayerPrefab(GameObject* aDefaultPlayerPrefab);
	GameObject& getDefaultPlayerPrefab() const;

	void setRole(const NetworkRole aRole);
	NetworkRole getRole() const;

	GameObject* instantiate(int aPrefabID, Transform aTransform = Transform());
	void destroy(GameObject& aObject);

private:
	void startServer();
	void startClient();
	void startHost();

		GameObject* instantiatePlayer(NetworkPacket packet);
	void destroyPlayer(SLNet::RakNetGUID playerID);

	std::vector<std::reference_wrapper<GameObject>>& getGameObjects();

	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	NetworkRole mRole;										  ///< The role of the network manager.
	int mTickRate;											  ///< The tick rate for sending packets.
	std::unique_ptr<GameObject> mDefaultPlayerPrefab;		  ///< The default player prefab.
	std::vector<std::reference_wrapper<GameObject>> mObjects; ///< The list of game objects managed by the network
															  ///< manager.

	std::unique_ptr<NetworkServer> mServer; ///< The server for the network manager.
	std::unique_ptr<NetworkClient> mClient; ///< The client for the network manager.
	std::unique_ptr<NetworkHost> mHost;		///< The host for the network manager.
};

#endif // NETWORKMANAGER_H