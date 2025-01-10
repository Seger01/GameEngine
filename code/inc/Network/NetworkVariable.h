/**
 * @file NetworkVariable.h
 * @brief Defines the NetworkVariableBase and NetworkVariable classes for network serialization.
 */

#ifndef NETWORKVARIABLE_H
#define NETWORKVARIABLE_H

#include "Network/INetworkBehaviour.h"
#include "Network/NetworkInformation.h"

#include <stdexcept>

/**
 * @class NetworkVariableBase
 * @brief Base class for network variables, providing serialization and dirty state management.
 */
class NetworkVariableBase
{
public:
	/**
	 * @brief Default constructor. Initializes the network variable ID.
	 */
	NetworkVariableBase() : mNetworkVariableID(-1) {}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~NetworkVariableBase() = default;

	/**
	 * @brief Serializes the network variable to a bit stream.
	 * @param stream The bit stream to serialize to.
	 */
	virtual void serialize(SLNet::BitStream& stream) const = 0;

	/**
	 * @brief Deserializes the network variable from a bit stream.
	 * @param stream The bit stream to deserialize from.
	 */
	virtual void deserialize(SLNet::BitStream& stream) = 0;

	/**
	 * @brief Gets the type ID of the network variable.
	 * @return The type ID.
	 */
	virtual uint32_t getTypeId() const = 0;

	/**
	 * @brief Sets the network variable ID.
	 * @param aNetworkVariableID The network variable ID to set.
	 */
	void setNetworkVariableID(int aNetworkVariableID) { mNetworkVariableID = aNetworkVariableID; }

	/**
	 * @brief Gets the network variable ID.
	 * @return The network variable ID.
	 */
	int getNetworkVariableID() const { return mNetworkVariableID; }

protected:
	int mNetworkVariableID; ///< The network variable ID.
};

/**
 * @class NetworkVariable
 * @brief Template class for network variables, providing type-specific serialization.
 *
 * @note The templated type must implement the INetworkSerializable interface.
 * @tparam INetworkSerializableTemplate The type of the network variable.
 */
template <typename INetworkSerializableTemplate> class NetworkVariable : public NetworkVariableBase
{
public:
	/**
	 * @brief Constructor. Registers the network variable with the owner.
	 * @param aOwner The owner of the network variable.
	 * @throws std::runtime_error if the owner is nullptr.
	 */
	NetworkVariable(INetworkBehaviour* aOwner) : NetworkVariableBase()
	{
		if (aOwner)
		{
			aOwner->RegisterNetworkVariable(this);
		}
		else
		{
			throw std::runtime_error("NetworkVariable::NetworkVariable() aOwner is nullptr");
		}
	}

	/**
	 * @brief Gets the value of the network variable.
	 * @return The value of the network variable.
	 */
	INetworkSerializableTemplate& getValue() { return mValue; }

	/**
	 * @brief Sets the value of the network variable.
	 * @param aValue The value to set.
	 */
	void setValue(INetworkSerializableTemplate aValue) { mValue = aValue; }

	/**
	 * @brief Serializes the network variable to a bit stream.
	 * @param stream The bit stream to serialize to.
	 */
	void serialize(SLNet::BitStream& stream) const override { mValue.serialize(stream); }

	/**
	 * @brief Deserializes the network variable from a bit stream.
	 * @param stream The bit stream to deserialize from.
	 */
	void deserialize(SLNet::BitStream& stream) override { mValue.deserialize(stream); }

	/**
	 * @brief Gets the type ID of the network variable.
	 * @return The type ID.
	 */
	uint32_t getTypeId() const override { return GetTypeId<INetworkSerializableTemplate>(); }

private:
	INetworkSerializableTemplate mValue; ///< The value of the network variable.
};

#endif // NETWORKVARIABLE_H