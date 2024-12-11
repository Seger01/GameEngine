/**
 * @file INetworkSerializable.h
 * @brief Interface for network serializable objects.
 *
 * This file contains the declaration of the INetworkSerializable interface,
 * which provides methods for serializing and deserializing objects over a network.
 */

#ifndef INETWORKSERIALIZABLE_H
#define INETWORKSERIALIZABLE_H

#include "Network/NetworkInformation.h"
#include "Network/NetworkRegister.h"

#include <cstdint>
#include <slikenet/BitStream.h>

/**
 * @class INetworkSerializable
 * @brief Interface for objects that can be serialized and deserialized over a network.
 *
 * The INetworkSerializable interface provides pure virtual methods for serializing
 * and deserializing
 *
 * When implementing this interface, add "REGISTER_NETWORK_SERIALIZABLE([ConcreteNetworkSerializable]);" to the .cpp
 * file.
 */
class INetworkSerializable {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~INetworkSerializable() = default;

	/**
	 * @brief Serializes the object to a bit stream.
	 * @param stream The bit stream to serialize to.
	 */
	virtual void serialize(SLNet::BitStream& stream) const = 0;

	/**
	 * @brief Deserializes the object from a bit stream.
	 * @param stream The bit stream to deserialize from.
	 */
	virtual void deserialize(SLNet::BitStream& stream) = 0;
};

#endif // INETWORKSERIALIZABLE_H
