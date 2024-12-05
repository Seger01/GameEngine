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
	virtual void onNetworkSpawn();

	void RegisterNetworkVariable(NetworkVariableBase* variable);

	std::vector<std::reference_wrapper<NetworkVariableBase>> GetNetworkVariables();

	bool isOwner();

	void destroy();
	uint8_t getNetworkBehaviourID() const;

private:
    bool mIsOwner;
    bool mIsOwnerSet;
	uint8_t mNetworkBehaviourID;
	static int networkBehaviourIDCounter;

    protected:
		std::vector<std::reference_wrapper<NetworkVariableBase>> mNetworkVariables;

	private:
		friend class NetworkObject;
};

#endif // NETWORKBEHAVIOUR_H