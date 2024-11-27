#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

#include "Components/IBehaviourScript.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkVariable.h"

#include <functional>
#include <stdexcept>
#include <vector>

class NetworkVariableBase;

class INetworkBehaviour : public IBehaviourScript {
public:
    INetworkBehaviour(std::string aTag = "defaultNetworkBehaviour") : IBehaviourScript(aTag) {}
    virtual ~INetworkBehaviour() = default;
    void initialize() {
        if (!mGameObject->hasComponent<NetworkObject>()) {
            throw std::runtime_error(
                "INetworkBehaviour must be attached to a GameObject with a NetworkObject component");
        } else {
            mGameObject->getComponents<NetworkObject>()[0]->addNetworkBehaviour(this);
            mNetworkBehaviourID = mGameObject->getComponents<NetworkObject>()[0]->getNetworkBehaviours().size() - 1;
        }
    }
    virtual void serverRpc() { throw std::runtime_error("INetworkBehaviour::serverRpc() not implemented"); }
    virtual void clientRpc() { throw std::runtime_error("INetworkBehaviour::clientRpc() not implemented"); }
    virtual void OnNetworkSpawn() { throw std::runtime_error("INetworkBehaviour::OnNetworkSpawn() not implemented"); }
    void RegisterNetworkVariable(NetworkVariableBase* variable) {
        mNetworkVariables.emplace_back(variable);
        variable->setNetworkVariableID(mNetworkVariables.size() - 1);
    }
    std::vector<NetworkVariableBase*> GetNetworkVariables() { return mNetworkVariables; }

private:
    bool mIsOwner;
    int mNetworkBehaviourID;
    std::vector<NetworkVariableBase*> mNetworkVariables;

private:
    friend class NetworkObject;
    void setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }
};

#endif // NETWORKBEHAVIOUR_H