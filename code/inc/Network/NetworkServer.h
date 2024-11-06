#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "GameObject.h"
#include "NetworkClient.h"
#include <list>
#include <memory>
#include <slikenet/peerinterface.h>
#include <vector>

class NetworkServer {
public:
    NetworkServer();
    void handleClientConnection();
    void receiveGameState();
    void sendGameState();
    void update(std::vector<GameObject*>& aGameObjects);

private:
    std::list<NetworkClient> mConnectedClients;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mPeer;
    static constexpr const char* SERVER_ADDRESS = "192.168.1.100";
    static constexpr unsigned short SERVER_PORT = 60001;
};

#endif // NETWORKSERVER_H