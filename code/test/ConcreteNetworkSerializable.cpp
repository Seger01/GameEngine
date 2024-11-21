#include "ConcreteNetworkSerializable.h"

// Register the type
REGISTER_NETWORK_SERIALIZABLE(ConcreteNetworkSerializable);

void ConcreteNetworkSerializable::Serialize(SLNet::BitStream& stream) const {
    stream.Write(mValue);
    stream.Write(mFloat);
}

void ConcreteNetworkSerializable::Deserialize(SLNet::BitStream& stream) {
    stream.Read(mValue);
    stream.Read(mFloat);
}