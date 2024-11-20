#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

#include "Components/IBehaviourScript.h"
#include "Network/NetworkVariable.h"

#include <functional>
#include <vector>

class INetworkBehaviour {
public:
    void serverRpc();
    void clientRpc();
    virtual void OnNetworkSpawn() = 0;
    void RegisterNetworkVariable(NetworkVariableBase* variable) { mNetworkVariables.emplace_back(variable); }

private:
    friend class NetworkManager;
    friend class NetworkServer;
    friend class NetworkClient;
    std::vector<NetworkVariableBase*> mNetworkVariables;
};

#endif // NETWORKBEHAVIOUR_H