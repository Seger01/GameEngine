#ifndef NETWORKHOST_H
#define NETWORKHOST_H

#include "Network/NetworkClient.h"
#include "Network/NetworkServer.h"

#include <memory>
#include <vector>

class GameObject;
class NetworkHost {
public:
	NetworkHost(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate);
	void update();

private:
    std::unique_ptr<NetworkServer> mServer;
    std::unique_ptr<NetworkClient> mClient;
};

#endif // NETWORKHOST_H