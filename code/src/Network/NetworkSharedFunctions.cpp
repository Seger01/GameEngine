#include "Network/NetworkSharedFunctions.h"

#include <iostream>

void NetworkSharedFunctions::makeBitStream(SLNet::BitStream& aBitStream) {
    NetworkPacket networkPacket;
    aBitStream.Reset();
    aBitStream.Write(networkPacket.messageID);
    aBitStream.Write(networkPacket.networkObjectID);
    aBitStream.Write(networkPacket.prefabID);
    aBitStream.Write(networkPacket.timestamp);
    aBitStream.Write(networkPacket.clientGUID);
    aBitStream.Write(networkPacket.ISerializableID);
    aBitStream.Write(networkPacket.networkBehaviourID);
    aBitStream.Write(networkPacket.networkVariableID);
}

NetworkPacket NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream) {
    NetworkPacket networkPacket;
    aBitStream.Read(networkPacket);
    return networkPacket;
}

void NetworkSharedFunctions::setBitStreamNetworkPacket(SLNet::BitStream& aBitStream,
                                                       const NetworkPacket& aNetworkPacket) {
    SLNet::BitSize_t WriteOffset = aBitStream.GetWriteOffset();
    aBitStream.SetWriteOffset(0);
    aBitStream.Write(aNetworkPacket.messageID);
    aBitStream.Write(aNetworkPacket.networkObjectID);
    aBitStream.Write(aNetworkPacket.prefabID);
    aBitStream.Write(aNetworkPacket.timestamp);
    aBitStream.Write(aNetworkPacket.clientGUID);
    aBitStream.Write(aNetworkPacket.ISerializableID);
    aBitStream.Write(aNetworkPacket.networkBehaviourID);
    aBitStream.Write(aNetworkPacket.networkVariableID);
    aBitStream.SetWriteOffset(WriteOffset); // Reset write offset
}
