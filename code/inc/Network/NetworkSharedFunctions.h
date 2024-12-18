/**
 * @file NetworkSharedFunctions.h
 * @brief Contains the declaration of the NetworkSharedFunctions class.
 */

#ifndef NETWORKSHAREDFUNCTIONS_H
#define NETWORKSHAREDFUNCTIONS_H

#include "Network/NetworkInformation.h"
#include <chrono>
#include <slikenet/BitStream.h>

/**
 * @class NetworkSharedFunctions
 * @brief Provides shared network functions for the NetworkServer and NetworkClient classes.
 */
class NetworkSharedFunctions {
public:
	static void reserveNetworkPacketBits(SLNet::BitStream& aBitStream);

	static NetworkPacket getBitStreamData(SLNet::BitStream& aBitStream);
    static void setBitStreamNetworkPacket(SLNet::BitStream& aBitStream, const NetworkPacket& aNetworkPacket);
};

#endif // NETWORKSHAREDFUNCTIONS_H