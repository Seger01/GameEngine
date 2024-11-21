#ifndef INETWORKSERIALIZABLE_H
#define INETWORKSERIALIZABLE_H

#include "Network/NetworkInformation.h"
#include "Network/NetworkRegister.h"

#include <cstdint>
#include <slikenet/BitStream.h>

class INetworkSerializable {
public:
    virtual ~INetworkSerializable() = default;

    // Virtual methods for serialization/deserialization
    virtual void Serialize(SLNet::BitStream& stream) const = 0;
    virtual void Deserialize(SLNet::BitStream& stream) = 0;
};

#endif // INETWORKSERIALIZABLE_H
