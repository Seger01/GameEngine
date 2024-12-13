/**
 * @file INetworkBehaviour.cpp
 * @brief Implements the INetworkBehaviour class for network-related behaviour scripts.
 */

#include "Network/INetworkBehaviour.h"

#include "Network/NetworkObject.h"
#include "Network/NetworkVariable.h"

#include <stdexcept>

int INetworkBehaviour::networkBehaviourIDCounter = 0;

/**
 * @brief Constructs a new INetworkBehaviour object.
 *
 * @param aTag The tag associated with this network behaviour.
 */
INetworkBehaviour::INetworkBehaviour(std::string aTag) : IBehaviourScript(aTag), mIsOwner(false), mIsOwnerSet(false)
{
	mNetworkBehaviourID = networkBehaviourIDCounter++;
}

/**
 * @brief Placeholder for server RPC implementation.
 *
 * @throws std::runtime_error Always throws as this function is not implemented.
 */
void INetworkBehaviour::serverRpc() { throw std::runtime_error("INetworkBehaviour::serverRpc() not implemented"); }

/**
 * @brief Placeholder for client RPC implementation.
 *
 * @throws std::runtime_error Always throws as this function is not implemented.
 */
void INetworkBehaviour::clientRpc() { throw std::runtime_error("INetworkBehaviour::clientRpc() not implemented"); }

/**
 * @brief Placeholder for network spawn handling.
 *
 * @throws std::runtime_error Always throws as this function is not implemented.
 */
void INetworkBehaviour::onNetworkSpawn()
{
	throw std::runtime_error("INetworkBehaviour::OnNetworkSpawn() not implemented");
}

/**
 * @brief Registers a network variable.
 *
 * @param variable Pointer to the network variable to register.
 */
void INetworkBehaviour::RegisterNetworkVariable(NetworkVariableBase* variable)
{
	mNetworkVariables.push_back(*variable);
	variable->setNetworkVariableID(mNetworkVariables.size() - 1);
}

/**
 * @brief Retrieves the list of network variables.
 *
 * @return std::vector<std::reference_wrapper<NetworkVariableBase>> List of network variables.
 */
std::vector<std::reference_wrapper<NetworkVariableBase>> INetworkBehaviour::GetNetworkVariables()
{
	return mNetworkVariables;
}

/**
 * @brief Checks if the current object is the owner.
 *
 * @return true If the current object is the owner.
 * @return false Otherwise.
 * @throws std::runtime_error If the NetworkObject is not found.
 */
bool INetworkBehaviour::isOwner()
{
	return mGameObject->getComponents<NetworkObject>()[0].get().isOwner();
	if (mIsOwnerSet)
	{
		return mIsOwner;
	}
	else
	{
		try
		{

			NetworkObject& networkObject = mGameObject->getComponents<NetworkObject>()[0];
			mIsOwner = networkObject.isOwner();
			mIsOwnerSet = true;
			return mIsOwner;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("INetworkBehaviour::isOwner() NetworkObject not found");
		}
	}
}

/**
 * @brief Destroys the network behaviour.
 */
void INetworkBehaviour::destroy() { mGameObject->removeComponent(this); }

/**
 * @brief Gets the network behaviour ID.
 *
 * @return uint8_t The network behaviour ID.
 */
uint8_t INetworkBehaviour::getNetworkBehaviourID() const { return mNetworkBehaviourID; }
