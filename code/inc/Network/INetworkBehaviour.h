/**
 * @file INetworkBehaviour.h
 * @brief Defines the INetworkBehaviour class for network-related behaviour scripts.
 */

#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

#include "Components/IBehaviourScript.h"

#include <string>
#include <vector>

class NetworkVariableBase;

/**
 * @class INetworkBehaviour
 * @brief Base class for network behaviour scripts.
 *
 * @note When adding a NetworkVariable to a NetworkBehaviour make sure to call RegisterNetworkVariable(this) in the
 * NetworkVariable constructor. Also make a copy constructor where you reset mNetworkVariables and add each
 * NetworkVariable.
 */
class INetworkBehaviour : public IBehaviourScript
{
public:
	INetworkBehaviour(std::string aTag = "defaultNetworkBehaviour");
	virtual ~INetworkBehaviour() = default;
	INetworkBehaviour(const INetworkBehaviour& other);

	virtual void serverRpc();
	virtual void clientRpc();
	virtual void onNetworkSpawn();

	void RegisterNetworkVariable(NetworkVariableBase* variable);
	std::vector<std::reference_wrapper<NetworkVariableBase>> GetNetworkVariables();

	bool isOwner();

	void destroy();

private:
	bool mIsOwner;						  ///< Indicates if the current object is the owner.
	bool mIsOwnerSet;					  ///< Indicates if the owner status has been set.

protected:
	std::vector<std::reference_wrapper<NetworkVariableBase>> mNetworkVariables; ///< List of network variables.
};

#endif // NETWORKBEHAVIOUR_H