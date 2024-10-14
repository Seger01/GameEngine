#include "NetworkServer.h"

void NetworkServer::HandleClientConnections(NetworkManager &manager)
{
    manager.Update();
}

void NetworkServer::SyncObjects(SLNet::RakPeerInterface *peer)
{
    for (auto &obj : gameObjects)
    {
        obj.SyncState(peer, SLNet::UNASSIGNED_SYSTEM_ADDRESS);
    }
}