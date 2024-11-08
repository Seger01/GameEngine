#ifndef NETWORKINFORMATION_H
#define NETWORKINFORMATION_H

#define SERVER_PORT 60001
#define CLIENT_PORT 60002

#include <slikenet/MessageIdentifiers.h>

enum class NetworkRole { UNASSIGNED, CLIENT, SERVER, HOST };

enum NetworkMessage {
    ID_TRANSFORM_PACKET = ID_USER_PACKET_ENUM + 1,
};

#endif // NETWORKINFORMATION_H