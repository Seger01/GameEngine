#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

#include "Components/IBehaviourScript.h"
#include "EngineBravo.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkVariable.h"

#include <functional>
#include <stdexcept>
#include <vector>

class NetworkVariableBase;

class INetworkBehaviour : public IBehaviourScript {
public:
    INetworkBehaviour(std::string aTag = "defaultNetworkBehaviour");
    virtual ~INetworkBehaviour() = default;

    virtual void serverRpc();
    virtual void clientRpc();
    virtual void OnNetworkSpawn();

    void RegisterNetworkVariable(NetworkVariableBase* variable);

    std::vector<NetworkVariableBase*> GetNetworkVariables();

    bool isOwner();

    void destroy();
    int getNetworkBehaviourID() const;

private:
    bool mIsOwner;
    bool mIsOwnerSet;
    int mNetworkBehaviourID;
    static int networkBehaviourIDCounter;
    std::vector<NetworkVariableBase*> mNetworkVariables;

private:
    friend class NetworkObject;
};

#endif // NETWORKBEHAVIOUR_H