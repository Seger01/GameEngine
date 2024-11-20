#include "Network/NetworkSharedFunctions.h"

void NetworkSharedFunctions::makeBitStream(SLNet::BitStream& aBitStream, SLNet::MessageID aMessageID) {
    NetworkPacket networkPacket;
    aBitStream.Reset();
    aBitStream.Write(networkPacket);
}

void NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream) {
    aBitStream.IgnoreBytes(sizeof(NetworkPacket));
}

void NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream, NetworkPacket& aNetworkPacket) {
    aBitStream.Read(aNetworkPacket);
}

void NetworkSharedFunctions::setBitStreamGUID(SLNet::BitStream& aBitStream, SLNet::RakNetGUID aGUID) {
    NetworkPacket networkPacket;
    aBitStream.Read(networkPacket);
    aBitStream.SetReadOffset(0);
    networkPacket.clientGUID = aGUID;
    // Find the position of the GUID and time in the bitstream
    SLNet::BitSize_t aWriteOffset = aBitStream.GetWriteOffset();
    aBitStream.SetWriteOffset(0);
    aBitStream.Write(networkPacket);
    aBitStream.SetWriteOffset(aWriteOffset); // Reset write offset
}
