/**
 * @file NetworkClient.cpp
 * @brief Implementation of the NetworkClient class.
 */

#include "NetworkClient.h"

#include "Network/INetworkBehaviour.h"
#include "Network/NetworkInformation.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkSharedFunctions.h"
#include "Network/NetworkTransform.h"
#include "Network/NetworkVariable.h"

#include "Engine/EngineBravo.h"

#include <functional>
#include <iostream>
#include <regex>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>

/**
 * @brief Constructs a NetworkClient object.
 * @param aObjects Reference to a vector of GameObjects.
 * @param aTickRate Tick rate for sending packets.
 */
NetworkClient::NetworkClient(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate = 60)
	: mClient(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance), mIsConnected(false),
	  mIsConnecting(false), mServerAddress("0.0.0.0"), mObjects(aObjects), mTickRate(aTickRate),
	  mLastSendPacketsTime(std::chrono::steady_clock::now())
{
	SLNet::SocketDescriptor sd(CLIENT_PORT, 0);
	sd.socketFamily = AF_INET;
	SLNet::StartupResult result = mClient->Startup(1, &sd, 1);
	if (result != SLNet::RAKNET_STARTED)
	{
		throw std::runtime_error("Failed to start client");
	}
}

/**
 * @brief Destructor for NetworkClient.
 */
NetworkClient::~NetworkClient()
{
	if (mClient->IsActive())
	{
		mClient->CloseConnection(mClient->GetMyGUID(), true);
		mClient->Shutdown(100);
	}
}

/**
 * @brief Connects to the server.
 */
void NetworkClient::connectToServer()
{
	if (mIsConnected || mIsConnecting)
	{
		return;
	}
	if (mServerAddress == "0.0.0.0")
	{
		throw std::runtime_error("Server address not set");
	}
	SLNet::ConnectionAttemptResult result = mClient->Connect(mServerAddress.c_str(), SERVER_PORT, nullptr, 0);
	if (result != SLNet::CONNECTION_ATTEMPT_STARTED)
	{
		throw std::runtime_error("Failed to start connection attempt");
	}
	mIsConnecting = true;
}

/**
 * @brief Updates the network client, handling incoming packets and sending packets.
 */
void NetworkClient::update()
{
	if (!mClient->IsActive())
	{
		throw std::runtime_error("Client is not running");
	}
	handleIncomingPackets();
	sendPackets();
}

/**
 * @brief Discovers available servers.
 */
void NetworkClient::discoverServers()
{
	mClient->Ping("255.255.255.255", SERVER_PORT, false); // Send discovery ping to the broadcast address
	mServerAddresses.clear();
}

/**
 * @brief Gets the list of discovered server addresses.
 * @return Reference to a vector of server addresses.
 */
std::vector<std::string>& NetworkClient::getServerAddresses() { return mServerAddresses; }

/**
 * @brief Sets the server address to connect to.
 * @param aServerAddress The server address.
 */
void NetworkClient::setServerAddress(std::string aServerAddress)
{
	const std::regex ipPattern(
		R"(\b((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\b)");
	if (!std::regex_match(aServerAddress, ipPattern))
	{
		throw std::runtime_error("Invalid IP address");
	}

	mServerAddress = aServerAddress;
}

/**
 * @brief Checks if the client is connected to the server.
 * @return True if connected, false otherwise.
 */
bool NetworkClient::isConnected() const { return mIsConnected; }

/**
 * @brief Sends packets to the server at the specified tick rate.
 */
void NetworkClient::sendPackets()
{
	auto now = std::chrono::steady_clock::now();
	auto frameTime = std::chrono::milliseconds(1000 / mTickRate);
	auto elapsed = now - mLastSendPacketsTime;
	if (now - mLastSendPacketsTime < frameTime)
	{
		return;
	}
	sendTransform();
	sendCustomSerialize();
	mLastSendPacketsTime = now;
}

/**
 * @brief Sends a bitstream to the server.
 * @param aBitStream The bitstream to send.
 */
void NetworkClient::sendToServer(SLNet::BitStream& aBitStream)
{
	mClient->Send(&aBitStream, MEDIUM_PRIORITY, PacketReliability::RELIABLE, 0, mServerGUID, false);
}

/**
 * @brief Sends transform data of owned game objects to the server.
 */
void NetworkClient::sendTransform()
{
	for (GameObject& gameObject : mObjects)
	{
		NetworkObject& networkObject = gameObject.getComponents<NetworkObject>()[0];
		if (!networkObject.isOwner())
		{
			continue;
		}
		if (gameObject.hasComponent<NetworkTransform>())
		{
			Transform transform = gameObject.getTransform();
			NetworkTransform& networkTransform = gameObject.getComponents<NetworkTransform>()[0];
			SLNet::BitStream bs;
			NetworkSharedFunctions::reserveNetworkPacketBits(bs);
			NetworkPacket networkPacket;
			networkPacket.messageID = NetworkMessage::ID_TRANSFORM_PACKET;
			networkPacket.networkObjectID = networkObject.getNetworkObjectID();
			NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
			if (networkTransform.getSendPositionX())
			{
				bs.Write(transform.position.x);
			}
			if (networkTransform.getSendPositionY())
			{
				bs.Write(transform.position.y);
			}
			if (networkTransform.getSendRotation())
			{
				bs.Write(transform.rotation);
			}
			if (networkTransform.getSendScaleX())
			{
				bs.Write(transform.scale.x);
			}
			if (networkTransform.getSendScaleY())
			{
				bs.Write(transform.scale.y);
			}
			sendToServer(bs);
		}
	}
}

/**
 * @brief Sends player initialization data to the server.
 */
void NetworkClient::sendPlayerInit()
{
	SLNet::BitStream bs;
	NetworkSharedFunctions::reserveNetworkPacketBits(bs);
	NetworkPacket networkPacket;
	networkPacket.messageID = NetworkMessage::ID_PLAYER_INIT;
	networkPacket.clientGUID = mClient->GetMyGUID();
	NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);

	sendToServer(bs);
}

/**
 * @brief Sends custom serialized data of owned game objects to the server.
 */
void NetworkClient::sendCustomSerialize()
{
	for (GameObject& gameObject : mObjects)
	{
		auto networkObject = gameObject.getComponents<NetworkObject>()[0];
		if (!networkObject.get().isOwner())
		{
			continue;
		}
		if (!gameObject.hasComponent<INetworkBehaviour>())
		{
			continue;
		}
		for (INetworkBehaviour& networkBehaviour : gameObject.getComponents<INetworkBehaviour>())
		{
			for (int networkVariableCounter = 0; networkVariableCounter < networkBehaviour.GetNetworkVariables().size();
				 networkVariableCounter++)
			{
				SLNet::BitStream bs;
				NetworkSharedFunctions::reserveNetworkPacketBits(bs);
				NetworkPacket networkPacket;
				networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_CUSTOM_SERIALIZE;
				networkPacket.networkObjectID = gameObject.getComponents<NetworkObject>()[0].get().getNetworkObjectID();
				networkPacket.ISerializableID =
					networkBehaviour.GetNetworkVariables().at(networkVariableCounter).get().getTypeId();
				networkPacket.SetTimeStampNow();
				networkPacket.clientGUID = gameObject.getComponents<NetworkObject>()[0].get().getClientGUID();
				networkPacket.networkBehaviourID = networkBehaviour.getNetworkBehaviourID();
				networkPacket.networkVariableID = networkVariableCounter;
				networkBehaviour.GetNetworkVariables().at(networkVariableCounter).get().serialize(bs);
				NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
				sendToServer(bs);
			}
		}
	}
}

/**
 * @brief Handles incoming packets from the server.
 */
void NetworkClient::handleIncomingPackets()
{
	SLNet::Packet* packet;
	for (packet = mClient->Receive(); packet; mClient->DeallocatePacket(packet), packet = mClient->Receive())
	{
		switch (packet->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			mIsConnected = true;
			mIsConnecting = false;
			mServerGUID = packet->guid;
			sendPlayerInit();
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			mIsConnecting = false;
			mIsConnected = false;
			break;
		case ID_CONNECTION_LOST:
			mIsConnected = false;
			break;
		case ID_UNCONNECTED_PONG:
		{
			SLNet::BitStream bs(packet->data, packet->length, false);
			NetworkSharedFunctions::getBitStreamData(bs);
			std::string serverIp;
			serverIp = packet->systemAddress.ToString(false);
			mServerAddresses.push_back(serverIp);
			break;
		}
		case ID_UNCONNECTED_PING:
		{
		}
		case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
		{
		}
		case (SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET:
			handleTransform(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT:
			handlePlayerInstantiation(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_PLAYER_DESTROY:
			handlePlayerDestruction(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_CUSTOM_SERIALIZE:
			handleCustomSerialize(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_SPAWN_PREFAB:
			handleSpawnPrefab(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_DESPAWN_PREFAB:
			handleDespawnPrefab(packet);
			break;
		default:
			std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
			break;
		}
	}
}

/**
 * @brief Handles transform packets from the server.
 * @param aPacket The packet containing transform data.
 */
void NetworkClient::handleTransform(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);

	SLNet::RakNetGUID clientGuid;
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);
	clientGuid = networkPacket.clientGUID;

	for (GameObject& gameObject : mObjects)
	{
		NetworkObject& networkObject = gameObject.getComponents<NetworkObject>()[0];
		if (networkObject.isOwner())
		{
			continue;
		}
		if (networkObject.getClientGUID() != clientGuid)
		{
			continue;
		}
		if (gameObject.hasComponent<NetworkTransform>())
		{
			Transform transform = gameObject.getTransform();
			NetworkTransform& networkTransform = gameObject.getComponents<NetworkTransform>()[0];
			if (networkTransform.getSendPositionX())
			{
				bs.Read(transform.position.x);
			}
			if (networkTransform.getSendPositionY())
			{
				bs.Read(transform.position.y);
			}
			if (networkTransform.getSendRotation())
			{
				bs.Read(transform.rotation);
			}
			if (networkTransform.getSendScaleX())
			{
				bs.Read(transform.scale.x);
			}
			if (networkTransform.getSendScaleY())
			{
				bs.Read(transform.scale.y);
			}
			gameObject.setTransform(transform);
		}
	}
}

/**
 * @brief Handles player instantiation packets from the server.
 * @param aPacket The packet containing player instantiation data.
 */
void NetworkClient::handlePlayerInstantiation(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);

	GameObject* player = EngineBravo::getInstance().getNetworkManager().instantiatePlayer(networkPacket); // Instantiate
																										  // client-side
																										  // player
	if (!player)
	{
		std::cout << "Player already exists\n";
		return;
	}
	std::vector<std::reference_wrapper<NetworkObject>> networkObjects = player->getComponents<NetworkObject>();
	if (networkObjects.size() == 0)
	{
		throw std::runtime_error("Player does not have a NetworkObject component");
	}
	if (networkPacket.clientGUID == mClient->GetMyGUID())
	{
		networkObjects[0].get().setOwner(true); // This client owns the player object
	}
	else
	{
		networkObjects[0].get().setOwner(false); // This client does not own the player object
	}
}

/**
 * @brief Handles custom serialization packets from the server.
 * @param aPacket The packet containing custom serialization data.
 */
void NetworkClient::handleCustomSerialize(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);

	for (GameObject& gameObject : mObjects)
	{
		NetworkObject& networkObject = gameObject.getComponents<NetworkObject>()[0];
		if (networkPacket.clientGUID != networkObject.getClientGUID())
		{ // check client ID
			std::cout << "Client ID does not match\n";
			continue;
		}
		if (networkPacket.networkObjectID != networkObject.getNetworkObjectID())
		{ // check network object ID
			std::cout << "Network object ID does not match\n";
			continue;
		}
		for (INetworkBehaviour& networkBehaviour : gameObject.getComponents<INetworkBehaviour>())
		{
			if (networkBehaviour.getNetworkBehaviourID() != networkPacket.networkBehaviourID)
			{
				continue;
			}

			if (networkBehaviour.GetNetworkVariables().size() <= networkPacket.networkVariableID)
			{ // check network variable ID bounds
				continue;
			}
			if (networkBehaviour.GetNetworkVariables().at(networkPacket.networkVariableID).get().getTypeId() !=
				networkPacket.ISerializableID)
			{ // check network variable ID
				std::cout << "Network variable ID does not match\n";
				continue;
			}
			networkBehaviour.GetNetworkVariables().at(networkPacket.networkVariableID).get().deserialize(bs);
			return;
		}
	}
}

/**
 * @brief Handles player destruction packets from the server.
 * @param aPacket The packet containing player destruction data.
 */
void NetworkClient::handlePlayerDestruction(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);
	EngineBravo::getInstance().getNetworkManager().destroyPlayer(networkPacket.clientGUID);
}

/**
 * @brief Handles prefab spawn packets from the server.
 * @param aPacket The packet containing prefab spawn data.
 */
void NetworkClient::handleSpawnPrefab(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);
	auto iNetworkprefab = NetworkRegister::Instance().CreatePrefabInstance(networkPacket.prefabID);
	if (!iNetworkprefab)
	{
		throw std::runtime_error("Prefab not found");
	}
	GameObject* prefab = iNetworkprefab->createEnemyPrefab();
	if (prefab->hasComponent<NetworkObject>())
	{
		NetworkObject& networkObject = prefab->getComponents<NetworkObject>()[0].get();
		networkObject.setPrefabID(networkPacket.prefabID);
		networkObject.setNetworkObjectID(networkPacket.networkObjectID);
	}
	else
	{
		throw std::runtime_error("Prefab does not have a NetworkObject component");
	}
	EngineBravo::getInstance().getSceneManager().getCurrentScene().addPersistentGameObject(prefab);
}

/**
 * @brief Handles prefab despawn packets from the server.
 * @param aPacket The packet containing prefab despawn data.
 */
void NetworkClient::handleDespawnPrefab(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.getComponents<NetworkObject>()[0].get().getNetworkObjectID() == networkPacket.networkObjectID)
		{
			EngineBravo::getInstance().getSceneManager().getCurrentScene().requestGameObjectRemoval(&gameObject);
			break;
		}
	}
}