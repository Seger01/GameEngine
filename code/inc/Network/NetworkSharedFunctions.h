#ifndef NETWORKSHAREDFUNCTIONS_H
#define NETWORKSHAREDFUNCTIONS_H

#include "Network/NetworkInformation.h"

#include <chrono>
#include <slikenet/BitStream.h>

class NetworkSharedFunctions {
public:
private:
    friend class NetworkServer;
    friend class NetworkClient;

private:
    static void makeBitStream(SLNet::BitStream& aBitStream, SLNet::MessageID aMessageID);

    static void getBitStreamData(SLNet::BitStream& aBitStream);
    static void getBitStreamData(SLNet::BitStream& aBitStream, NetworkPacket& aNetworkPacket);
    static void setBitStreamGUID(SLNet::BitStream& aBitStream, SLNet::RakNetGUID aGUID);
};

#endif // NETWORKSHAREDFUNCTIONS_H