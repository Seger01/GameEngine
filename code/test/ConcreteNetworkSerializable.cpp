#include "ConcreteNetworkSerializable.h"

// Register the type
REGISTER_NETWORK_SERIALIZABLE(ConcreteNetworkSerializable);

void ConcreteNetworkSerializable::serialize(SLNet::BitStream& stream) const {
    stream.Write(mValue);
    stream.Write(mFloat);
}

void ConcreteNetworkSerializable::deserialize(SLNet::BitStream& stream) {
    stream.Read(mValue);
    stream.Read(mFloat);
}