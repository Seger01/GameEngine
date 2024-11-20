#ifndef NETWORKSHAREDFUNCTIONS_H
#define NETWORKSHAREDFUNCTIONS_H

#include <slikenet/BitStream.h>

#include <chrono>

class NetworkSharedFunctions {
public:
private:
    friend class NetworkServer;
    friend class NetworkClient;

private:
    static void makeBitStream(SLNet::BitStream& aBitStream, SLNet::MessageID aMessageID);
    static void getBitStreamData(SLNet::BitStream& aBitStream);
    static void getBitStreamData(SLNet::BitStream& aBitStream, SLNet::RakNetGUID& aGUID);
    static void getBitStreamData(SLNet::BitStream& aBitStream, std::chrono::milliseconds::rep& aTimeStamp);
    static void getBitStreamData(SLNet::BitStream& aBitStream, std::chrono::milliseconds::rep& aTimeStamp,
                                 SLNet::RakNetGUID& aGUID);
    static void setBitStreamGUID(SLNet::BitStream& aBitStream, SLNet::RakNetGUID aGUID);
};

#endif // NETWORKSHAREDFUNCTIONS_H