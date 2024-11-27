#include "Network/NetworkHost.h"

NetworkHost::NetworkHost(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate)
{
	mServer = std::make_unique<NetworkServer>(aObjects, aTickRate);
	mClient = std::make_unique<NetworkClient>(aObjects, aTickRate);
}

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
	mClient->setServerAddress("127.0.0.1"); // Automatically connect to local server
    mClient->connectToServer();
}
