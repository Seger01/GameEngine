/**
 * @file NetworkHost.cpp
 * @brief The file contains the implementation of the NetworkHost class.
 */
#include "Network/NetworkHost.h"

#include "Network/NetworkClient.h"
#include "Network/NetworkServer.h"

/**
 * @brief Constructs a new NetworkHost object.
 * 
 * @param aObjects A vector of GameObject references to be managed by the network host.
 * @param aTickRate The tick rate for the network updates.
 */
NetworkHost::NetworkHost(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate)
{
	mServer = std::make_unique<NetworkServer>(aObjects, aTickRate);
	mClient = std::make_unique<NetworkClient>(aObjects, aTickRate);
	mClient->setServerAddress("127.0.0.1"); // Automatically connect to local server
	mClient->connectToServer();
}

/**
 * @brief Updates the network host, including both server and client.
 */
void NetworkHost::update()
{
	if (mServer)
	{
		mServer->update();
	}
	if (mClient)
	{
		mClient->update();
	}
}
