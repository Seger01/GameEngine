#ifndef INETWORKSERIALIZABLE_H
#define INETWORKSERIALIZABLE_H

#include "Network/NetworkInformation.h"

#include <cstdint>
#include <slikenet/BitStream.h>

class INetworkSerializable {
public:
    virtual ~INetworkSerializable() = default;

    // Virtual methods for serialization/deserialization
    virtual void Serialize(SLNet::BitStream& stream) const = 0;
    virtual void Deserialize(SLNet::BitStream& stream) = 0;

    // Returns the unique type ID
    virtual uint32_t GetTypeID() const = 0;

    // AUTO_REGISTER_TYPE([concreteImplementation]) // Automatically register this type
};

#endif // INETWORKSERIALIZABLE_H
