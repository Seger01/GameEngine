/**
 * @file NetworkInformation.h
 * @brief Contains definitions for network roles, messages, and packet structures.
 */

#ifndef NETWORKINFORMATION_H
#define NETWORKINFORMATION_H

#define SERVER_PORT 60001 ///< The port number for the server.
#define CLIENT_PORT 60002 ///< The port number for the client.

#include "Network/NetworkRegister.h"

#include <chrono>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>

/**
 * @enum NetworkRole
 * @brief Defines the role of the network participant.
 */
enum class NetworkRole { UNASSIGNED, CLIENT, SERVER, HOST };

/**
 * @enum NetworkMessage
 * @brief Defines custom network message identifiers.
 */
enum NetworkMessage
{
	ID_TRANSFORM_PACKET = ID_USER_PACKET_ENUM + 1,
	ID_PLAYER_INIT = ID_TRANSFORM_PACKET + 1,
	ID_PLAYER_DESTROY = ID_PLAYER_INIT + 1,
	ID_CUSTOM_SERIALIZE = ID_PLAYER_DESTROY + 1,
	ID_SPAWN_PREFAB = ID_CUSTOM_SERIALIZE + 1,
	ID_DESPAWN_PREFAB = ID_SPAWN_PREFAB + 1,
};

/**
 * @struct NetworkPacket
 * @brief Represents a network packet with various attributes.
 */
struct NetworkPacket {
	SLNet::MessageID messageID;	  ///< The message ID of the packet.
	uint16_t networkObjectID;	  ///< The network object ID.
	uint32_t prefabID;			  ///< The prefab ID.
	uint64_t timestamp;			  ///< The timestamp of the packet.
	SLNet::RakNetGUID clientGUID; ///< The GUID of the client.
	uint32_t ISerializableID;	  ///< The serializable ID.
	uint8_t networkBehaviourID;	  ///< The network behaviour ID.
	uint8_t networkVariableID;	  ///< The network variable ID.

	/**
	 * @brief Default constructor for NetworkPacket.
	 */
	NetworkPacket()
		: messageID(0), networkObjectID(UINT16_MAX), prefabID(UINT16_MAX), timestamp(0), clientGUID(SLNet::UNASSIGNED_RAKNET_GUID),
		  ISerializableID(UINT16_MAX), networkBehaviourID(UINT8_MAX), networkVariableID(UINT8_MAX) {
	}

	/**
	 * @brief Sets the timestamp to the current time.
	 */
	void SetTimeStampNow() {
        auto now = std::chrono::steady_clock::now().time_since_epoch();
        timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    }
};

#endif // NETWORKINFORMATION_H