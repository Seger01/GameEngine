/**
 * @file INetworkBehaviour.cpp
 * @brief Implements the INetworkBehaviour class for network-related behaviour scripts.
 */

#include "Network/INetworkBehaviour.h"

#include "Network/NetworkObject.h"
#include "Network/NetworkVariable.h"

#include "Engine/EngineBravo.h"

#include <stdexcept>

int INetworkBehaviour::mNetworkBehaviourIDCounter = 0;

/**
 * @brief Constructs a new INetworkBehaviour object.
 *
 * @param aTag The tag associated with this network behaviour.
 */
INetworkBehaviour::INetworkBehaviour(const std::string& aTag)
	: IBehaviourScript(aTag), mIsOwner(false), mIsOwnerSet(false)
{
	mNetworkVariables.clear();
	// mNetworkBehaviourID = mNetworkBehaviourIDCounter++;
}

/**
 * @brief Copy constructor.
 *
 * @param other The INetworkBehaviour to copy.
 */
INetworkBehaviour::INetworkBehaviour(const INetworkBehaviour& other)
	: IBehaviourScript(other), mIsOwner(other.mIsOwner), mIsOwnerSet(other.mIsOwnerSet),
	  mNetworkBehaviourID(other.mNetworkBehaviourID)
{
	mNetworkVariables.clear();
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

/**
 * @brief Checks if the current object is the server.
 *
 * @return true If the current object is the server.
 * @return false Otherwise.
 */
bool INetworkBehaviour::isServer() { return EngineBravo::getInstance().getNetworkManager().isServer(); }

/**
 * @brief Destroys the network behaviour.
 *
 * @note This function is not implemented. It breaks the synchronization of the network behaviours
 */
// void INetworkBehaviour::destroy() { mGameObject->removeComponent(this); }

/**
 * @brief Gets the network behaviour ID.
 *
 * @return int The network behaviour ID.
 */
int INetworkBehaviour::getNetworkBehaviourID() const { return mNetworkBehaviourID; }

/**
 * @brief Sets the network behaviour ID.
 *
 * @param aNetworkBehaviourID The network behaviour ID to set.
 */
void INetworkBehaviour::setNetworkBehaviourID(uint32_t aNetworkBehaviourID)
{
	mNetworkBehaviourID = aNetworkBehaviourID;
}
