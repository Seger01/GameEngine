#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <vector>
#include "NetworkObject.h"
#include "NetworkManager.h"
#include <slikenet/peerinterface.h>

class NetworkServer
{
public:
    std::vector<NetworkObject> gameObjects;

    void HandleClientConnections(NetworkManager &manager);
    void SyncObjects(SLNet::RakPeerInterface *peer);
};

#endif // NETWORKSERVER_H