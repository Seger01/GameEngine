#include "Network/NetworkSharedFunctions.h"

void NetworkSharedFunctions::makeBitStream(SLNet::BitStream& aBitStream, SLNet::MessageID aMessageID) {
    aBitStream.Reset();
    aBitStream.Write(aMessageID);

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    auto timestamp = duration.count();

    aBitStream.Write(timestamp);

    SLNet::RakNetGUID placeholderGUID = SLNet::UNASSIGNED_RAKNET_GUID;
    aBitStream.Write(placeholderGUID);
}

void NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream) {
    aBitStream.IgnoreBytes(sizeof(SLNet::MessageID));
    aBitStream.IgnoreBytes(sizeof(std::chrono::milliseconds::rep));
    aBitStream.IgnoreBytes(sizeof(SLNet::RakNetGUID));
}

void NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream, SLNet::RakNetGUID& aGUID) {
    aBitStream.IgnoreBytes(sizeof(SLNet::MessageID));
    aBitStream.IgnoreBytes(sizeof(std::chrono::milliseconds::rep));
    aBitStream.Read(aGUID);
}

void NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream,
                                              std::chrono::milliseconds::rep& aTimeStamp) {
    aBitStream.IgnoreBytes(sizeof(SLNet::MessageID));
    aBitStream.Read(aTimeStamp);
    aBitStream.IgnoreBytes(sizeof(SLNet::RakNetGUID));
}

void NetworkSharedFunctions::getBitStreamData(SLNet::BitStream& aBitStream, std::chrono::milliseconds::rep& aTimeStamp,
                                              SLNet::RakNetGUID& aGUID) {
    SLNet::MessageID messageID;
    aBitStream.IgnoreBytes(sizeof(SLNet::MessageID));
    aBitStream.Read(aTimeStamp);
    aBitStream.Read(aGUID);
}

void NetworkSharedFunctions::setBitStreamGUID(SLNet::BitStream& aBitStream, SLNet::RakNetGUID aGUID) {
    // Find the position of the GUID and time in the bitstream
    size_t Position = sizeof(SLNet::MessageID) + sizeof(std::chrono::milliseconds::rep);
    SLNet::BitSize_t aWriteOffset = aBitStream.GetWriteOffset();
    aBitStream.SetWriteOffset(Position * 8); // Convert to bits
    aBitStream.Write(aGUID);
    aBitStream.SetWriteOffset(aWriteOffset); // Reset write offset
}
