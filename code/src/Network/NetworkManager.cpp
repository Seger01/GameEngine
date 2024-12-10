/**
 * @file NetworkManager.cpp
 * @brief Implementation of the NetworkManager class responsible for managing network roles and operations.
 */

#include "Network/NetworkManager.h"

#include "Network/NetworkObject.h"

#include "Engine/EngineBravo.h"
#include "Engine/SceneManager.h"

#include <stdexcept>

/**
 * @brief Constructs a NetworkManager with default role and tick rate.
 */
NetworkManager::NetworkManager() : mRole(NetworkRole::UNASSIGNED), mTickRate(60) {}

/**
 * @brief Starts the network based on the assigned role.
 */
void NetworkManager::startNetwork()
{
	if (mRole == NetworkRole::SERVER)
	{
		startServer();
	}
	else if (mRole == NetworkRole::CLIENT)
	{
		startClient();
	}
	else if (mRole == NetworkRole::HOST)
	{
		startHost();
	}
}

/**
 * @brief Shuts down the network manager.
 * @throws std::runtime_error Always throws as the function is not implemented.
 */
void NetworkManager::shutdown() { throw std::runtime_error("NetworkManager::shutdown() not implemented"); }

/**
 * @brief Initializes the network manager by starting the network.
 */
void NetworkManager::initialize() { startNetwork(); }

/**
 * @brief Updates the network manager based on the current role.
 */
void NetworkManager::update()
{
	if (mRole == NetworkRole::SERVER && mServer)
	{
		mServer->update();
	}
	else if (mRole == NetworkRole::CLIENT && mClient)
	{
		mClient->update();
	}
	else if (mRole == NetworkRole::HOST && mHost)
	{
		mHost->update();
	}
}

/**
 * @brief Gets the server instance.
 * @return Reference to the NetworkServer.
 * @throws std::runtime_error If the server is not running.
 */
NetworkServer& NetworkManager::getServer() const
{
	if (!mServer)
	{
		throw std::runtime_error("Server is not running");
	}
	return *mServer;
}

/**
 * @brief Gets the client instance.
 * @return Reference to the NetworkClient.
 * @throws std::runtime_error If the client is not running.
 */
NetworkClient& NetworkManager::getClient() const
{
	if (!mClient)
	{
		throw std::runtime_error("Client is not running");
	}
	return *mClient;
}

/**
 * @brief Gets the host instance.
 * @return Reference to the NetworkHost.
 * @throws std::runtime_error If the host is not running.
 */
NetworkHost& NetworkManager::getHost() const
{
	if (!mHost)
	{
		throw std::runtime_error("Host is not running");
	}
	return *mHost;
}

/**
 * @brief Checks if the current role is server.
 * @return True if the role is server, false otherwise.
 */
bool NetworkManager::isServer() const { return mRole == NetworkRole::SERVER; }

/**
 * @brief Checks if the current role is client.
 * @return True if the role is client, false otherwise.
 */
bool NetworkManager::isClient() const { return mRole == NetworkRole::CLIENT; }

/**
 * @brief Checks if the current role is host.
 * @return True if the role is host, false otherwise.
 */
bool NetworkManager::isHost() const { return mRole == NetworkRole::HOST; }

/**
 * @brief Checks if the network manager is networked (server, client, or host).
 * @return True if networked, false otherwise.
 */
bool NetworkManager::isNetworked() const { return isServer() || isClient() || isHost(); }

/**
 * @brief Checks if the network manager is connected.
 * @return True if connected, false otherwise.
 * @throws std::runtime_error If the role is host and the function is not implemented.
 */
bool NetworkManager::isConnected() const
{
	if (mRole == NetworkRole::SERVER)
	{
		return mServer->isConnected();
	}
	else if (mRole == NetworkRole::CLIENT)
	{
		return mClient->isConnected();
	}
	else if (mRole == NetworkRole::HOST)
	{
		throw std::runtime_error("NetworkManager::isConnected() isHost not implemented");
	}
	return false;
}

/**
 * @brief Sets the tick rate for the network manager.
 * @param aTickRate The tick rate to set.
 */
void NetworkManager::setTickRate(int aTickRate) { mTickRate = aTickRate; }

/**
 * @brief Gets the current tick rate.
 * @return The current tick rate.
 */
int NetworkManager::getTickRate() const { return mTickRate; }

/**
 * @brief Sets the default player prefab.
 * @param aDefaultPlayerPrefab Pointer to the default player prefab.
 */
void NetworkManager::setDefaultPlayerPrefab(GameObject* aDefaultPlayerPrefab)
{
	if (!aDefaultPlayerPrefab->hasComponent<NetworkObject>())
	{
		NetworkObject* networkObject = new NetworkObject();
		aDefaultPlayerPrefab->addComponent(networkObject);
	}
	mDefaultPlayerPrefab.reset(aDefaultPlayerPrefab);
}

/**
 * @brief Gets the default player prefab.
 * @return Reference to the default player prefab.
 */
GameObject& NetworkManager::getDefaultPlayerPrefab() const { return *mDefaultPlayerPrefab; }

/**
 * @brief Instantiates a player based on the provided network packet.
 * @param packet The network packet containing player information.
 * @return Pointer to the instantiated player.
 * @throws std::runtime_error If the player prefab is not set or does not have a NetworkObject component.
 */
GameObject* NetworkManager::instantiatePlayer(NetworkPacket packet)
{
	if (!mDefaultPlayerPrefab)
	{
		throw std::runtime_error("Player prefab not set.");
	}

	for (auto object : mObjects)
	{
		NetworkObject* networkObject = object.get().getComponents<NetworkObject>()[0];
		if (networkObject->getClientGUID() == packet.clientGUID) // Check if player already exists
		{
			return nullptr;
		}
	}

	GameObject* player = new GameObject(*mDefaultPlayerPrefab); // Clone prefab
	auto networkObjects = player->getComponents<NetworkObject>();
	if (networkObjects.size() == 0)
	{
		throw std::runtime_error("Player prefab does not have a NetworkObject component");
	}
	networkObjects[0]->setClientGUID(packet.clientGUID); // Assign unique ID to player
	if (packet.networkObjectID != -1)
	{
		networkObjects[0]->setNetworkObjectID(packet.networkObjectID);
	}
	else
	{
		networkObjects[0]->setNetworkObjectID(NetworkObject::networkObjectIDCounter++);
	}
	std::cout << "Player instantiated with network object ID: " << networkObjects[0]->getNetworkObjectID() << std::endl;
	networkObjects[0]->setPlayer(true);		  // Mark as player
	EngineBravo::getInstance().getSceneManager().getCurrentScene()->addPersistentGameObject(player);
	return player;
}

/**
 * @brief Destroys a player based on the provided player ID.
 * @param playerID The ID of the player to destroy.
 */
void NetworkManager::destroyPlayer(SLNet::RakNetGUID playerID)
{
	for (auto object : mObjects)
	{
		NetworkObject* networkObject = object.get().getComponents<NetworkObject>()[0];
		if (networkObject->getClientGUID() == playerID)
		{
			EngineBravo::getInstance().getSceneManager().getCurrentScene()->requestGameObjectRemoval(&object.get());
			return;
		}
	}
}

/**
 * @brief Sets the network role.
 * @param aRole The role to set.
 */
void NetworkManager::setRole(NetworkRole aRole) { mRole = aRole; }

/**
 * @brief Gets the current network role.
 * @return The current network role.
 */
NetworkRole NetworkManager::getRole() const { return mRole; }

/**
 * @brief Gets the list of game objects managed by the network manager.
 * @return Reference to the vector of game objects.
 */
std::vector<std::reference_wrapper<GameObject>>& NetworkManager::getGameObjects() { return mObjects; }

/**
 * @brief Starts the server.
 * @throws std::runtime_error If the client or host is already running, or if the server is already running.
 */
void NetworkManager::startServer()
{
	if (mClient || mHost)
	{
		throw std::runtime_error("Cannot start server when client or host is already running");
	}
	if (mServer)
	{
		throw std::runtime_error("Server is already running");
	}
	mServer = std::make_unique<NetworkServer>(mObjects, mTickRate);
}

/**
 * @brief Starts the client.
 * @throws std::runtime_error If the server or host is already running, or if the client is already running.
 */
void NetworkManager::startClient()
{
	if (mServer || mHost)
	{
		throw std::runtime_error("Cannot start client when server or host is already running");
	}
	if (mClient)
	{
		throw std::runtime_error("Client is already running");
	}
	mClient = std::make_unique<NetworkClient>(mObjects, mTickRate);
}

/**
 * @brief Starts the host.
 * @throws std::runtime_error If the server or client is already running, or if the host is already running.
 */
void NetworkManager::startHost() {
    if (mServer || mClient) {
        throw std::runtime_error("Cannot start host when server or client is already running");
    }
    if (mHost) {
        throw std::runtime_error("Host is already running");
    }
	mHost = std::make_unique<NetworkHost>(mObjects, mTickRate);
}

/**
 * @brief Adds a game object to the network manager.
 * @param aObject The game object to add.
 */
void NetworkManager::addObject(GameObject& aObject)
{
	auto it = std::find_if(mObjects.begin(), mObjects.end(),
						   [&aObject](const std::reference_wrapper<GameObject>& wrapper)
						   {
							   return &wrapper.get() == &aObject; // Compare addresses
						   });
	if (it == mObjects.end())
	{
		// Object has not been added yet
		mObjects.push_back(aObject);
	}
}

/**
 * @brief Removes a game object from the network manager.
 * @param aObject The game object to remove.
 */
void NetworkManager::removeObject(GameObject& aObject)
{
	auto it =
		std::remove_if(mObjects.begin(), mObjects.end(),
					   [&aObject](const std::reference_wrapper<GameObject>& obj) { return &obj.get() == &aObject; });
	if (it != mObjects.end())
	{
		mObjects.erase(it, mObjects.end());
	}
}

/**
 * @brief Gets the list of game objects managed by the network manager.
 * @return Const reference to the vector of game objects.
 */
const std::vector<std::reference_wrapper<GameObject>>& NetworkManager::getObjects() const { return mObjects; }

/**
 * @brief Clears all game objects managed by the network manager.
 */
void NetworkManager::clearObjects() { mObjects.clear(); }

/**
 * @brief Instantiates a prefab based on the provided prefab ID and transform.
 * @param aPrefabID The ID of the prefab to instantiate.
 * @param aTransform The transform to apply to the instantiated prefab.
 * @return Pointer to the instantiated prefab.
 */
GameObject* NetworkManager::instantiate(int aPrefabID, Transform aTransform)
{
	auto iNetworkprefab = NetworkRegister::Instance().CreatePrefabInstance(aPrefabID);
	if (!iNetworkprefab)
	{
		throw std::runtime_error("Prefab not found");
	}
	GameObject* prefab = iNetworkprefab->create();
	if (prefab->hasComponent<NetworkObject>())
	{
		NetworkObject* networkObject = prefab->getComponents<NetworkObject>()[0];
		networkObject->setPrefabID(aPrefabID);
	}
	else
	{
		throw std::runtime_error("Prefab does not have a NetworkObject component");
	}
	if (aTransform.position != Transform().position || aTransform.rotation != Transform().rotation ||
		aTransform.scale != Transform().scale)
	{
		prefab->setTransform(aTransform);
	}
	EngineBravo::getInstance().getSceneManager().getCurrentScene()->addPersistentGameObject(prefab);
	return prefab;
}

/**
 * @brief Instantiates a prefab based on the provided prefab ID and transform.
 * @param aNetworkPacket The network packet containing the prefab ID and network object ID.
 * @return Pointer to the instantiated prefab.
 */
GameObject* NetworkManager::instantiatePrefab(NetworkPacket aNetworkPacket)
{
	auto iNetworkprefab = NetworkRegister::Instance().CreatePrefabInstance(aNetworkPacket.prefabID);
	if (!iNetworkprefab)
	{
		throw std::runtime_error("Prefab not found");
	}
	GameObject* prefab = iNetworkprefab->create();
	if (prefab->hasComponent<NetworkObject>())
	{
		NetworkObject* networkObject = prefab->getComponents<NetworkObject>()[0];
		networkObject->setPrefabID(aNetworkPacket.prefabID);
		networkObject->setNetworkObjectID(aNetworkPacket.networkObjectID);
	}
	else
	{
		throw std::runtime_error("Prefab does not have a NetworkObject component");
	}
	EngineBravo::getInstance().getSceneManager().getCurrentScene()->addPersistentGameObject(prefab);
	return prefab;
}