#include "NetworkClient.h"

#include "Network/NetworkInformation.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkSharedFunctions.h"
#include "Network/NetworkTransform.h"

#include "Engine/EngineBravo.h"

#include <iostream>
#include <regex>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>

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
	std::cout << "Client started\n";
}

NetworkClient::~NetworkClient()
{
	if (mClient->IsActive())
	{
		mClient->CloseConnection(mClient->GetMyGUID(), true);
		mClient->Shutdown(100);
	}
}

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
}

void NetworkClient::update()
{
	if (!mClient->IsActive())
	{
		throw std::runtime_error("Client is not running");
	}
	handleIncomingPackets();
	sendPackets();
}

void NetworkClient::discoverServers()
{
	mClient->Ping("255.255.255.255", SERVER_PORT, false); // Send discovery ping to the broadcast address
	mServerAddresses.clear();
}

std::vector<std::string>& NetworkClient::getServerAddresses() { return mServerAddresses; }

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

bool NetworkClient::isConnected() const { return mIsConnected; }

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
	mLastSendPacketsTime = now;
}

void NetworkClient::requestPlayerInstantiation()
{
	SLNet::BitStream bs;
	NetworkSharedFunctions::makeBitStream(bs);
	NetworkPacket networkPacket;
	networkPacket.messageID = NetworkMessage::ID_PLAYER_INIT;
	networkPacket.clientGUID = mClient->GetMyGUID();
	NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);

	sendToServer(bs);
}

void NetworkClient::handleIncomingPackets()
{
	SLNet::Packet* packet;
	for (packet = mClient->Receive(); packet; mClient->DeallocatePacket(packet), packet = mClient->Receive())
	{
		switch (packet->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Connected to server.\n";
			mIsConnected = true;
			mIsConnecting = false;
			mServerGUID = packet->guid;
			requestPlayerInstantiation();
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			std::cout << "Connection attempt failed. Retrying...\n";
			mIsConnecting = false;
			mIsConnected = false;
			break;
		case ID_CONNECTION_LOST:
			std::cout << "Connection lost or disconnected.\n";
			mIsConnected = false;
			break;
		case ID_UNCONNECTED_PONG:
		{
			std::cout << "Got pong from " << packet->systemAddress.ToString() << std::endl;
			SLNet::BitStream bs(packet->data, packet->length, false);
			NetworkSharedFunctions::getBitStreamData(bs);
			std::string serverIp;
			serverIp = packet->systemAddress.ToString(false);
			mServerAddresses.push_back(serverIp);
			break;
		}
		case ID_UNCONNECTED_PING:
		{
			std::cout << "Got ping from " << packet->systemAddress.ToString() << std::endl;
		}
		case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
		{
			std::cout << "Got open connection ping from " << packet->systemAddress.ToString() << std::endl;
		}
		case (SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET:
			handleTransform(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT:
			std::cout << "Received player init packet\n";
			handlePlayerInstantiation(packet);
			break;
		case (SLNet::MessageID)NetworkMessage::ID_PLAYER_DESTROY:
			std::cout << "Received player destroy packet\n";
			handlePlayerDestruction(packet);
			break;
		default:
			std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
			break;
		}
	}
}

void NetworkClient::sendTransform()
{

	for (GameObject& gameObject : mObjects)
	{
		NetworkObject* networkObject = gameObject.getComponents<NetworkObject>()[0];
		if (!networkObject->isOwner())
		{
			continue;
		}
		if (gameObject.hasComponent<NetworkTransform>())
		{
			Transform transform = gameObject.getTransform();
			NetworkTransform* networkTransform = gameObject.getComponents<NetworkTransform>()[0];
			SLNet::BitStream bs;
			NetworkSharedFunctions::makeBitStream(bs);
			NetworkPacket networkPacket;
			networkPacket.messageID = NetworkMessage::ID_TRANSFORM_PACKET;
			networkPacket.networkObjectID = networkObject->getNetworkObjectID();
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
			sendToServer(bs);
		}
	}
}

void NetworkClient::handleTransform(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);

	SLNet::RakNetGUID clientGuid;
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);
	clientGuid = networkPacket.clientGUID;

	for (auto gameObject : mObjects)
	{
		NetworkObject* networkObject = gameObject.get().getComponents<NetworkObject>()[0];
		if (networkObject->isOwner())
		{
			continue;
		}
		if (networkObject->getClientGUID() != clientGuid)
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

void NetworkClient::handlePlayerInstantiation(SLNet::Packet* aPacket)
{
	SLNet::RakNetGUID playerID;
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);
	playerID = networkPacket.clientGUID;
	std::cout << "playerID: " << playerID.ToString() << std::endl;

	GameObject* player = EngineBravo::getInstance().getNetworkManager().instantiatePlayer(playerID); // Instantiate
																									 // client-side
																									 // player
	if (!player)
	{
		std::cout << "Player already exists\n";
		return;
	}
	std::vector<NetworkObject*> networkObjects = player->getComponents<NetworkObject>();
	if (networkObjects.size() == 0)
	{
		throw std::runtime_error("Player does not have a NetworkObject component");
	}
	if (playerID == mClient->GetMyGUID())
	{
		networkObjects[0]->setOwner(true); // This client owns the player object
	}
	else
	{
		networkObjects[0]->setOwner(false); // This client does not own the player object
	}
}

void NetworkClient::handlePlayerDestruction(SLNet::Packet* aPacket)
{
	SLNet::BitStream bs(aPacket->data, aPacket->length, false);
	NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);

	std::cout << "playerId to destroy: " << networkPacket.clientGUID.ToString() << std::endl;

	EngineBravo::getInstance().getNetworkManager().destroyPlayer(networkPacket.clientGUID);
}

void NetworkClient::sendToServer(SLNet::BitStream& aBitStream)
{
	mClient->Send(&aBitStream, MEDIUM_PRIORITY, PacketReliability::RELIABLE, 0, mServerGUID, false);
}
