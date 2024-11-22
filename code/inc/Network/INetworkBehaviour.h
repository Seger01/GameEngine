#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

#include "Components/IBehaviourScript.h"
#include "Network/NetworkObject.h"

#include <functional>
#include <stdexcept>
#include <vector>

class NetworkVariableBase;

class INetworkBehaviour : public IBehaviourScript {
public:
    void serverRpc();
    void clientRpc();
    virtual void OnNetworkSpawn() = 0;
    void RegisterNetworkVariable(NetworkVariableBase* variable) { mNetworkVariables.emplace_back(variable); }
    std::vector<NetworkVariableBase*> GetNetworkVariables() { return mNetworkVariables; }

protected:
    INetworkBehaviour() {
        if (!mGameObject->hasComponent<NetworkObject>()) {
            throw std::runtime_error(
                "INetworkBehaviour must be attached to a GameObject with a NetworkObject component");
        } else {
            mGameObject->getComponents<NetworkObject>()[0]->addNetworkBehaviour(this);
        }
    }

    bool mIsOwner;

private:
    friend class NetworkManager;
    friend class NetworkServer;
    friend class NetworkClient;
    friend class NetworkObject;
    std::vector<NetworkVariableBase*> mNetworkVariables;
    void setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }
};

#endif // NETWORKBEHAVIOUR_H