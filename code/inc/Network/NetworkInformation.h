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
};

enum class WritePermission { ReadOnly, ReadWrite };

template <typename T> struct AutoRegister {
    AutoRegister() { NetworkRegister::Instance().RegisterType<T>(); }
};

// Macro for automatic type registration
#define AUTO_REGISTER_TYPE(Type) static AutoRegister<Type> Type##_registrar;

struct NetworkPacket {
    SLNet::MessageID messageID;
    int networkObjectID;
    int prefabID;
    int ISerializableID;
    std::chrono::milliseconds::rep timestamp;
    SLNet::RakNetGUID clientGUID;

    NetworkPacket()
        : messageID(-1), networkObjectID(-1), prefabID(-1), ISerializableID(-1), timestamp(-1),
          clientGUID(SLNet::UNASSIGNED_RAKNET_GUID) {}
};
#endif // NETWORKINFORMATION_H