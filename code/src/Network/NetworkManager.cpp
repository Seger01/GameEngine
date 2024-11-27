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
		mServer->update(mObjects);
	}
	else if (mRole == NetworkRole::CLIENT && mClient)
	{
		mClient->update(mObjects);
	}
	else if (mRole == NetworkRole::HOST && mHost)
	{
		throw std::runtime_error("NetworkManager::update() isHost not implemented");
		// mHost->update();
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

GameObject* NetworkManager::instantiatePlayer(SLNet::RakNetGUID playerID)
{
	if (!mDefaultPlayerPrefab)
	{
		throw std::runtime_error("Player prefab not set.");
	}

	std::vector<GameObject*> persistantObjects =
		EngineBravo::getInstance().getSceneManager().getCurrentScene()->getPersistentGameObjects();
	for (auto object : persistantObjects)
	{
		if (!object->hasComponent<NetworkObject>())
		{
			continue;
		}
		NetworkObject* networkObject = object->getComponents<NetworkObject>()[0];
		if (networkObject->getClientID() == playerID)
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
	networkObjects[0]->setClientID(playerID); // Assign unique ID to player
	networkObjects[0]->setPlayer(true);		  // Mark as player
	EngineBravo::getInstance().getSceneManager().getCurrentScene()->addPersistentGameObject(player);
	return player;
}

void NetworkManager::destroyPlayer(SLNet::RakNetGUID playerID)
{
	std::vector<GameObject*> persistantObjects =
		EngineBravo::getInstance().getSceneManager().getCurrentScene()->getPersistentGameObjects();
	for (auto object : persistantObjects)
	{
		if (!object->hasComponent<NetworkObject>())
		{
			continue;
		}
		NetworkObject* networkObject = object->getComponents<NetworkObject>()[0];
		if (networkObject->getClientID() == playerID)
		{
			EngineBravo::getInstance().getSceneManager().getCurrentScene()->removePersistentGameObject(object);
			break;
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

void NetworkManager::startHost()
{
	if (mServer || mClient)
	{
		throw std::runtime_error("Cannot start host when server or client is already running");
	}
	if (mHost)
	{
		throw std::runtime_error("Host is already running");
	}
	mHost = std::make_unique<NetworkHost>();
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