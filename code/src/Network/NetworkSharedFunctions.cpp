#include "Network/NetworkSharedFunctions.h"

void NetworkSharedFunctions::makeBitStream(SLNet::BitStream& aBitStream) {
    NetworkPacket networkPacket;
    aBitStream.Reset();
    aBitStream.Write(networkPacket);
}

NetworkPacket NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream) {
    NetworkPacket networkPacket;
    aBitStream.Read(networkPacket);
    return networkPacket;
}

void NetworkSharedFunctions::setBitStreamNetworkPacket(SLNet::BitStream& aBitStream,
                                                       const NetworkPacket& aNetworkPacket) {
    SLNet::BitSize_t aWriteOffset = aBitStream.GetWriteOffset();
    aBitStream.SetWriteOffset(0);
    aBitStream.Write(aNetworkPacket);
    aBitStream.SetWriteOffset(aWriteOffset); // Reset write offset
}
