#ifndef NETWORKINFORMATION_H
#define NETWORKINFORMATION_H

#define SERVER_PORT 60001
#define CLIENT_PORT 60002

#include "Network/NetworkRegister.h"

#include <chrono>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>

enum class NetworkRole { UNASSIGNED, CLIENT, SERVER, HOST };

enum NetworkMessage {
    ID_TRANSFORM_PACKET = ID_USER_PACKET_ENUM + 1,
    ID_PLAYER_INIT = ID_TRANSFORM_PACKET + 1,
    ID_PLAYER_DESTROY = ID_PLAYER_INIT + 1,
    ID_CUSTOM_SERIALIZE = ID_PLAYER_DESTROY + 1,
};

enum class WritePermission { ReadOnly, ReadWrite };

struct NetworkPacket {
    SLNet::MessageID messageID;
    uint32_t networkObjectID;
    uint16_t prefabID;
    uint64_t timestamp;
    SLNet::RakNetGUID clientGUID;
	uint32_t ISerializableID;
	uint8_t networkBehaviourID;
	uint8_t networkVariableID;

    NetworkPacket()
        : messageID(0), networkObjectID(-1), prefabID(-1), timestamp(0), clientGUID(SLNet::UNASSIGNED_RAKNET_GUID),
          ISerializableID(-1), networkBehaviourID(-1), networkVariableID(-1) {}
    void SetTimeStampNow() {
        auto now = std::chrono::steady_clock::now().time_since_epoch();
        timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    }
};
#endif // NETWORKINFORMATION_H