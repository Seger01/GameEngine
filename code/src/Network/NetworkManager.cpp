#include "Network/NetworkManager.h"
#include "Network/NetworkObject.h"

#include "Engine/EngineBravo.h"
#include "Engine/SceneManager.h"

NetworkManager::NetworkManager() : mRole(NetworkRole::UNASSIGNED), mTickRate(60), mEnableSceneManagement(false) {}

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

void NetworkManager::shutdown() { throw std::runtime_error("NetworkManager::shutdown() not implemented"); }

void NetworkManager::initialize() { startNetwork(); }

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

NetworkServer& NetworkManager::getServer() const
{
	if (!mServer)
	{
		throw std::runtime_error("Server is not running");
	}
	return *mServer;
}

NetworkClient& NetworkManager::getClient() const
{
	if (!mClient)
	{
		throw std::runtime_error("Client is not running");
	}
	return *mClient;
}

NetworkHost& NetworkManager::getHost() const
{
	if (!mHost)
	{
		throw std::runtime_error("Host is not running");
	}
	return *mHost;
}

bool NetworkManager::isServer() const { return mRole == NetworkRole::SERVER; }

bool NetworkManager::isClient() const { return mRole == NetworkRole::CLIENT; }

bool NetworkManager::isHost() const { return mRole == NetworkRole::HOST; }

bool NetworkManager::isNetworked() const { return isServer() || isClient() || isHost(); }

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

void NetworkManager::setTickRate(int aTickRate) { mTickRate = aTickRate; }

int NetworkManager::getTickRate() const { return mTickRate; }

void NetworkManager::setEnableSceneManagement(bool aEnableSceneManagement)
{
	mEnableSceneManagement = aEnableSceneManagement;
}

bool NetworkManager::getEnableSceneManagement() const { return mEnableSceneManagement; }

void NetworkManager::setDefaultPlayerPrefab(GameObject* aDefaultPlayerPrefab)
{
	if (!aDefaultPlayerPrefab->hasComponent<NetworkObject>())
	{
		NetworkObject* networkObject = new NetworkObject();
		aDefaultPlayerPrefab->addComponent(networkObject);
	}
	mDefaultPlayerPrefab.reset(aDefaultPlayerPrefab);
}

GameObject& NetworkManager::getDefaultPlayerPrefab() const { return *mDefaultPlayerPrefab; }

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
	std::cout << "Player instantiated with network object ID: " << networkObjects[0]->getNetworkObjectID() << std::endl;
	networkObjects[0]->setPlayer(true);		  // Mark as player
	EngineBravo::getInstance().getSceneManager().getCurrentScene()->addPersistentGameObject(player);
	return player;
}

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

void NetworkManager::setRole(NetworkRole aRole) { mRole = aRole; }

NetworkRole NetworkManager::getRole() const { return mRole; }

std::vector<std::reference_wrapper<GameObject>>& NetworkManager::getGameObjects() { return mObjects; }

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

void NetworkManager::startHost() {
    if (mServer || mClient) {
        throw std::runtime_error("Cannot start host when server or client is already running");
    }
    if (mHost) {
        throw std::runtime_error("Host is already running");
    }
	mHost = std::make_unique<NetworkHost>(mObjects, mTickRate);
}

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

const std::vector<std::reference_wrapper<GameObject>>& NetworkManager::getObjects() const { return mObjects; }

void NetworkManager::clearObjects() { mObjects.clear(); }