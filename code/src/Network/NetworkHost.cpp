#include "Network/NetworkHost.h"

NetworkHost::NetworkHost(int aTickRate) {
    mServer = std::make_unique<NetworkServer>(aTickRate);
    mClient = std::make_unique<NetworkClient>(aTickRate);
}

void NetworkHost::update(std::vector<GameObject*>& aGameObjects) {
    if (mServer) {
        mServer->update(aGameObjects);
    }
    if (mClient) {
        mClient->update(aGameObjects);
    }
}
