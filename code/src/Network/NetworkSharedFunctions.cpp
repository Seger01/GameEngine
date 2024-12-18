/**
 * @file NetworkSharedFunctions.cpp
 * @brief Contains the implementation of the NetworkSharedFunctions class.
 */
#include "Network/NetworkSharedFunctions.h"

/**
 * @brief Reserves the network packet bits in the bit stream.
 *
 * @note clears the bit stream
 * @param aBitStream The bit stream to reserve the bits in.
 */
void NetworkSharedFunctions::reserveNetworkPacketBits(SLNet::BitStream& aBitStream)
{
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

/**
 * @brief Extracts data from a bit stream into a network packet.
 *
 * @param aBitStream The bit stream to extract data from.
 * @return NetworkPacket The extracted network packet.
 */
NetworkPacket NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream) {
    NetworkPacket networkPacket;
    aBitStream.Read(networkPacket.messageID);
    aBitStream.Read(networkPacket.networkObjectID);
    aBitStream.Read(networkPacket.prefabID);
    aBitStream.Read(networkPacket.timestamp);
    aBitStream.Read(networkPacket.clientGUID);
    aBitStream.Read(networkPacket.ISerializableID);
    aBitStream.Read(networkPacket.networkBehaviourID);
    aBitStream.Read(networkPacket.networkVariableID);
    return networkPacket;
}

/**
 * @brief Sets the bit stream with the given network packet data.
 *
 * @param aBitStream The bit stream to be set.
 * @param aNetworkPacket The network packet containing the data.
 */
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
