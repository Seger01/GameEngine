/**
 * @file NetworkServer.cpp
 * @brief Implementation of the NetworkServer class for handling server-side network operations.
 */

#include "NetworkServer.h"

#include "Network/INetworkBehaviour.h"
#include "Network/NetworkInformation.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkSharedFunctions.h"
#include "Network/NetworkTransform.h"
#include "Network/NetworkVariable.h"

#include "Engine/EngineBravo.h"

#include <iostream>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/PacketLogger.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>
#include <string>

/**
 * @brief Constructs a NetworkServer object.
 * @param aObjects Reference to a vector of GameObjects.
 * @param aTickRate Tick rate for the server (default is 60).
 */
NetworkServer::NetworkServer(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate = 60)
	: mServer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance),
	  mLastSendPacketsTime(std::chrono::steady_clock::now()), mObjects(aObjects), mTickRate(aTickRate)
{
	SLNet::SocketDescriptor sd(SERVER_PORT, 0);
	sd.socketFamily = AF_INET;
	SLNet::StartupResult result = mServer->Startup(10, &sd, 1);
	mServer->SetMaximumIncomingConnections(10);
	if (result != SLNet::RAKNET_STARTED)
	{
		throw std::runtime_error("Failed to start server");
	}
}

/**
 * @brief Updates the server, handling incoming packets and sending updates.
 */
void NetworkServer::update()
{
	if (!mServer->IsActive())
	{
		throw std::runtime_error("Server is not running");
	}
	handleIncomingPackets();
	sendPackets();
}

/**
 * @brief Checks if the server is connected.
 * @return True if the server is active, false otherwise.
 */
bool NetworkServer::isConnected() const { return mServer->IsActive(); }

/**
 * @brief Handles incoming packets from clients.
 */
void NetworkServer::handleIncomingPackets()
{
	SLNet::Packet* packet;
	for (packet = mServer->Receive(); packet; mServer->DeallocatePacket(packet), packet = mServer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			std::cout << "A connection is incoming.\n";
			spawnPlayerForNewClient(packet->guid);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full.\n";
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "A client has disconnected.\n";
			handleClientDisconnect(packet->guid);
			break;
		case ID_CONNECTION_LOST:
			std::cout << "A client lost the connection.\n";
			handleClientDisconnect(packet->guid);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET:
			handleTransform(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT:
			std::cout << "Received player instantiation message.\n";
			handlePlayerInit(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_CUSTOM_SERIALIZE:
			handleCustomSerialize(packet);
			break;
		default:
			std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
			break;
		}
	}
}

/**
 * @brief Sends packets to clients at the specified tick rate.
 */
void NetworkServer::sendPackets()
{
	auto now = std::chrono::steady_clock::now();
	auto frameTime = std::chrono::milliseconds(1000 / mTickRate);
	if (now - mLastSendPacketsTime < frameTime)
	{
		return;
	}
	sendTransform();
	sendCustomSerialize();
	mLastSendPacketsTime = now;
}

/**
 * @brief Sends transform data of game objects to all clients.
 */
void NetworkServer::sendTransform()
{
	for (GameObject& gameObject : mObjects)
	{
		if (!gameObject.hasComponent<NetworkTransform>())
		{
			continue;
		}

		Transform transform = gameObject.getTransform();
		NetworkTransform* networkTransform = gameObject.getComponents<NetworkTransform>()[0];
		SLNet::BitStream bs;
		NetworkSharedFunctions::makeBitStream(bs);
		NetworkPacket networkPacket;
		NetworkObject* networkObject = gameObject.getComponents<NetworkObject>()[0];
		networkPacket.messageID = NetworkMessage::ID_TRANSFORM_PACKET;
		networkPacket.networkObjectID = networkObject->getNetworkObjectID();
		networkPacket.clientGUID = networkObject->getClientGUID();
		NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);

		if (networkTransform->getSendPositionX())
		{
			bs.Write(transform.position.x);
		}
		if (networkTransform->getSendPositionY())
		{
			bs.Write(transform.position.y);
		}
		if (networkTransform->getSendRotation())
		{
			bs.Write(transform.rotation);
		}
		if (networkTransform->getSendScaleX())
		{
			bs.Write(transform.scale.x);
		}
		if (networkTransform->getSendScaleY())
		{
			bs.Write(transform.scale.y);
		}

		sendToAllClients(bs);
	}
}

/**
 * @brief Sends custom serialized data of game objects to all clients.
 */
void NetworkServer::sendCustomSerialize()
{
	for (auto gameObject : mObjects)
	{
		if (!gameObject.get().hasComponent<INetworkBehaviour>())
		{
			continue;
		}
		for (INetworkBehaviour* networkBehaviour : gameObject.get().getComponents<INetworkBehaviour>())
		{
			for (int i = 0; i < networkBehaviour->GetNetworkVariables().size(); i++)
			{
				// if (!networkBehaviour->GetNetworkVariables().at(i).get().isDirty()) // If data is not changed do not send it
				// {
				// 	continue;
				// }
				SLNet::BitStream bs;
				NetworkSharedFunctions::makeBitStream(bs);
				NetworkPacket networkPacket;
				networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_CUSTOM_SERIALIZE;
				networkPacket.networkObjectID =
					gameObject.get().getComponents<NetworkObject>()[0]->getNetworkObjectID();
				networkPacket.ISerializableID = networkBehaviour->GetNetworkVariables().at(i).get().getTypeId();
				networkPacket.SetTimeStampNow();
				networkPacket.clientGUID = gameObject.get().getComponents<NetworkObject>()[0]->getClientGUID();
				networkPacket.networkBehaviourID = networkBehaviour->getNetworkBehaviourID();
				networkPacket.networkVariableID = i;

				networkBehaviour->GetNetworkVariables().at(i).get().serialize(bs);
				NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
				sendToAllClients(bs);
			}
		}
	}
}

/**
 * @brief Sends player initialization data to all clients.
 * @param playerID The GUID of the player to initialize.
 */
void NetworkServer::sendPlayerInit(SLNet::RakNetGUID playerID)
{
	std::cout << "Sending player instantiation message to all clients.\n";
	SLNet::BitStream bs;
	NetworkSharedFunctions::makeBitStream(bs);
	NetworkPacket networkPacket;
	networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT;
	networkPacket.clientGUID = playerID;

	auto persistantObjects = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getPersistentGameObjects();
	for (auto player : persistantObjects)
	{
		if (!player->hasComponent<NetworkObject>())
		{
			continue;
		}
		auto networkObject = player->getComponents<NetworkObject>()[0];
		if (networkObject->getClientGUID() != playerID)
		{
			continue;
		}
		networkPacket.clientGUID = networkObject->getClientGUID();
		networkPacket.networkObjectID = networkObject->getNetworkObjectID();
		networkPacket.SetTimeStampNow();
		NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
		sendToAllClients(bs);
		return;
	}
}

/**
 * @brief Sends a spawn prefab message to all clients.
 * @param aObject The object to spawn.
 */
void NetworkServer::sendPrefabSpawn(NetworkObject& aObject)
{
	SLNet::BitStream bs;
	NetworkSharedFunctions::makeBitStream(bs);
	NetworkPacket networkPacket;
	networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_SPAWN_PREFAB;
	networkPacket.networkObjectID = aObject.getNetworkObjectID();
	networkPacket.prefabID = aObject.getPrefabID();
	networkPacket.SetTimeStampNow();
	NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
	sendToAllClients(bs);
}

/**
 * @brief Sends a despawn message to all clients.
 * @param aObject The object to despawn.
 */
void NetworkServer::sendPrefabDespawn(NetworkObject& aObject)
{
	SLNet::BitStream bs;
	NetworkSharedFunctions::makeBitStream(bs);
	NetworkPacket networkPacket;
	networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_DESPAWN_PREFAB;
	networkPacket.networkObjectID = aObject.getNetworkObjectID();
	networkPacket.SetTimeStampNow();
	NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
	sendToAllClients(bs);
}

/**
 * @brief Handles incoming transform packets from clients.
 * @param aPacket The packet containing transform data.
 */
void NetworkServer::handleTransform(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkSharedFunctions::getBitStreamData(bs);

	for (auto gameObject : mObjects)
	{
		auto networkObject = gameObject.get().getComponents<NetworkObject>()[0];
		if (networkObject->getClientGUID() != aPacket->guid)
		{
			continue;
		}
		if (gameObject.get().hasComponent<NetworkTransform>())
		{
			Transform transform = gameObject.get().getTransform();
			NetworkTransform* networkTransform = gameObject.get().getComponents<NetworkTransform>()[0];
			if (networkTransform->getSendPositionX())
			{
				bs.Read(transform.position.x);
			}
			if (networkTransform->getSendPositionY())
			{
				bs.Read(transform.position.y);
			}
			if (networkTransform->getSendRotation())
			{
				bs.Read(transform.rotation);
			}
			if (networkTransform->getSendScaleX())
			{
				bs.Read(transform.scale.x);
			}
			if (networkTransform->getSendScaleY())
			{
				bs.Read(transform.scale.y);
			}
			gameObject.get().setTransform(transform);
		}
	}
}

/**
 * @brief Handles incoming custom serialized packets from clients.
 * @param aPacket The packet containing custom serialized data.
 */
void NetworkServer::handleCustomSerialize(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);

	for (auto gameObject : mObjects)
	{
		NetworkObject* networkObject = gameObject.get().getComponents<NetworkObject>()[0];
		if (networkObject->isOwner()) // if owner do not overwrite with value from other instances
		{
			continue;
		}
		if (networkPacket.clientGUID != networkObject->getClientGUID())
		{ // check client ID
			continue;
		}
		if (networkPacket.networkObjectID != networkObject->getNetworkObjectID())
		{ // check network object ID
			continue;
		}
		for (auto networkBehaviour : gameObject.get().getComponents<INetworkBehaviour>())
		{
			if (networkBehaviour->getNetworkBehaviourID() != networkPacket.networkBehaviourID)
			{ // check network
			  // behaviour ID
				continue;
			}
			if (networkBehaviour->GetNetworkVariables().at(networkPacket.networkVariableID).get().getTypeId() !=
				networkPacket.ISerializableID)
			{ // check network variable ID
				continue;
			}
			networkBehaviour->GetNetworkVariables().at(networkPacket.networkVariableID).get().deserialize(bs);
		}
	}
}

/**
 * @brief Handles player initialization packets from clients.
 * @param aPacket The packet containing player initialization data.
 */
void NetworkServer::handlePlayerInit(SLNet::Packet* aPacket)
{
	SLNet::RakNetGUID clientID = aPacket->guid;
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);
	if (EngineBravo::getInstance().getNetworkManager().getRole() != NetworkRole::HOST)
	{																					 // if host don't add
		EngineBravo::getInstance().getNetworkManager().instantiatePlayer(networkPacket); // Server-side
																						 // instantiation
	}

	sendPlayerInit(clientID); // Send instantiation message to all clients
}

/**
 * @brief Handles client disconnection events.
 * @param clientID The GUID of the disconnected client.
 */
void NetworkServer::handleClientDisconnect(SLNet::RakNetGUID clientID)
{
	EngineBravo::getInstance().getNetworkManager().destroyPlayer(clientID);

	SLNet::BitStream bs;
	NetworkSharedFunctions::makeBitStream(bs);
	NetworkPacket networkPacket;
	networkPacket.messageID = NetworkMessage::ID_PLAYER_DESTROY;
	networkPacket.clientGUID = clientID;
	NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
	sendToAllClients(bs);
}

/**
 * @brief Sends a bitstream to all connected clients.
 * @param aBitStream The bitstream to send.
 */
void NetworkServer::sendToAllClients(SLNet::BitStream& aBitStream)
{
	DataStructures::List<SLNet::SystemAddress> addresses;
	DataStructures::List<SLNet::RakNetGUID> guids;
	mServer->GetSystemList(addresses, guids);
	for (int i = 0; i < addresses.Size(); i++)
	{
		mServer->Send(&aBitStream, PacketPriority::MEDIUM_PRIORITY, PacketReliability::RELIABLE, 0, guids[i], false);
	}
}

/**
 * @brief Spawns a player for a new client.
 * @param playerID The GUID of the new client.
 */
void NetworkServer::spawnPlayerForNewClient(SLNet::RakNetGUID playerID)
{
	std::cout << "Sending all players to each client" << std::endl;
	SLNet::BitStream bs;
	NetworkSharedFunctions::makeBitStream(bs);
	NetworkPacket networkPacket;
	networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT;
	networkPacket.clientGUID = playerID;

	auto persistantObjects = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getPersistentGameObjects();
	for (auto player : persistantObjects)
	{
		if (!player->hasComponent<NetworkObject>())
		{
			continue;
		}
		auto networkObject = player->getComponents<NetworkObject>()[0];
		networkPacket.clientGUID = networkObject->getClientGUID();
		networkPacket.networkObjectID = networkObject->getNetworkObjectID();
		networkPacket.SetTimeStampNow();
		NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
		sendToAllClients(bs);
	}
}
