#define protected public
#define private public
#include "Network/INetworkBehaviour.h"
#include "Network/INetworkSerializable.h"
#include "Network/NetworkClient.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkRegister.h"
#include "Network/NetworkServer.h"
#include "Network/NetworkSharedFunctions.h"
#include "Network/NetworkTransform.h"
#undef protected
#undef private

#include <gtest/gtest.h>

#include "ConcreteNetworkBehaviour.h"
#include "ConcreteNetworkSerializable.h"

class NetworkTransformTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Set up necessary objects before each test.
	}
};

TEST_F(NetworkTransformTest, DefaultConstructor)
{
	NetworkTransform nt;
	EXPECT_FALSE(nt.getSendPositionX());
	EXPECT_FALSE(nt.getSendPositionY());
	EXPECT_FALSE(nt.getSendRotation());
	EXPECT_FALSE(nt.getSendScaleX());
	EXPECT_FALSE(nt.getSendScaleY());
}

TEST_F(NetworkTransformTest, ParameterizedConstructor)
{
	NetworkTransform nt(true, true, true, true, true);
	EXPECT_TRUE(nt.getSendPositionX());
	EXPECT_TRUE(nt.getSendPositionY());
	EXPECT_TRUE(nt.getSendRotation());
	EXPECT_TRUE(nt.getSendScaleX());
	EXPECT_TRUE(nt.getSendScaleY());
}

TEST_F(NetworkTransformTest, SettersAndGetters)
{
	NetworkTransform nt;
	nt.setSendPositionX(true);
	nt.setSendPositionY(true);
	nt.setSendRotation(true);
	nt.setSendScaleX(true);
	nt.setSendScaleY(true);

	EXPECT_TRUE(nt.getSendPositionX());
	EXPECT_TRUE(nt.getSendPositionY());
	EXPECT_TRUE(nt.getSendRotation());
	EXPECT_TRUE(nt.getSendScaleX());
	EXPECT_TRUE(nt.getSendScaleY());

	nt.setSendPositionX(false);
	nt.setSendPositionY(false);
	nt.setSendRotation(false);
	nt.setSendScaleX(false);
	nt.setSendScaleY(false);

	EXPECT_FALSE(nt.getSendPositionX());
	EXPECT_FALSE(nt.getSendPositionY());
	EXPECT_FALSE(nt.getSendRotation());
	EXPECT_FALSE(nt.getSendScaleX());
	EXPECT_FALSE(nt.getSendScaleY());
}

TEST_F(NetworkTransformTest, Clone)
{
	NetworkTransform nt(true, false, true, false, true);
	std::unique_ptr<Component> clone = nt.clone();
	NetworkTransform* clonedNt = dynamic_cast<NetworkTransform*>(clone.get());

	ASSERT_NE(clonedNt, nullptr);
	EXPECT_TRUE(clonedNt->getSendPositionX());
	EXPECT_FALSE(clonedNt->getSendPositionY());
	EXPECT_TRUE(clonedNt->getSendRotation());
	EXPECT_FALSE(clonedNt->getSendScaleX());
	EXPECT_TRUE(clonedNt->getSendScaleY());
}

class NetworkObjectTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Set up necessary objects before each test.
	}
};

TEST_F(NetworkObjectTest, DefaultConstructor)
{
	NetworkObject no;
	EXPECT_FALSE(no.isOwner());
	EXPECT_EQ(no.getClientGUID(), SLNet::UNASSIGNED_RAKNET_GUID);
}

TEST_F(NetworkObjectTest, SetAndGetClientID)
{
	NetworkObject no;
	SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
	no.setClientGUID(clientID);
	EXPECT_EQ(no.getClientGUID(), clientID);
}

TEST_F(NetworkObjectTest, SetAndGetOwner)
{
	NetworkObject no;
	no.setOwner(true);
	EXPECT_TRUE(no.isOwner());
	no.setOwner(false);
	EXPECT_FALSE(no.isOwner());
}

TEST_F(NetworkObjectTest, Clone)
{
	NetworkObject no;
	no.setOwner(true);
	SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
	no.setClientGUID(clientID);

	std::unique_ptr<Component> clone = no.clone();
	NetworkObject* clonedNo = dynamic_cast<NetworkObject*>(clone.get());

	ASSERT_NE(clonedNo, nullptr);
	EXPECT_TRUE(clonedNo->isOwner());
	EXPECT_EQ(clonedNo->getClientGUID(), clientID);
}

TEST_F(NetworkObjectTest, CopyConstructor)
{
	NetworkObject no;
	no.setOwner(true);
	SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
	no.setClientGUID(clientID);

	NetworkObject copyNo(no);

	EXPECT_TRUE(copyNo.isOwner());
	EXPECT_EQ(copyNo.getClientGUID(), clientID);
}

TEST_F(NetworkObjectTest, CopyAssignment)
{
	NetworkObject no;
	no.setOwner(true);
	SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
	no.setClientGUID(clientID);

	NetworkObject assignedNo;
	assignedNo = no;

	EXPECT_TRUE(assignedNo.isOwner());
	EXPECT_EQ(assignedNo.getClientGUID(), clientID);
}

TEST_F(NetworkObjectTest, MoveConstructor)
{
	NetworkObject no;
	no.setOwner(true);
	SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
	no.setClientGUID(clientID);

	NetworkObject movedNo(std::move(no));

	EXPECT_TRUE(movedNo.isOwner());
	EXPECT_EQ(movedNo.getClientGUID(), clientID);
	EXPECT_FALSE(no.isOwner());
	EXPECT_EQ(no.getClientGUID(), SLNet::UNASSIGNED_RAKNET_GUID);
}

TEST_F(NetworkObjectTest, MoveAssignment)
{
	NetworkObject no;
	no.setOwner(true);
	SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
	no.setClientGUID(clientID);

	NetworkObject movedNo;
	movedNo = std::move(no);

	EXPECT_TRUE(movedNo.isOwner());
	EXPECT_EQ(movedNo.getClientGUID(), clientID);
	EXPECT_FALSE(no.isOwner());
	EXPECT_EQ(no.getClientGUID(), SLNet::UNASSIGNED_RAKNET_GUID);
}

class NetworkBehaviourTest : public ::testing::Test
{
protected:
	void SetUp() override {}
};

TEST_F(NetworkBehaviourTest, RegisterNetworkVariable)
{
	GameObject gameObject;
	gameObject.addComponent<NetworkObject>();
	gameObject.addComponent<ConcreteNetworkBehaviour>();

	INetworkBehaviour& behaviour = gameObject.getComponents<INetworkBehaviour>()[0];

	const auto& variables = behaviour.GetNetworkVariables();
	ASSERT_EQ(variables.size(), 1);
}

class INetworkSerializableTest : public ::testing::Test
{
protected:
	void SetUp() override {}
};

TEST_F(INetworkSerializableTest, TestSerializableRegistration) {
    auto& registry = NetworkRegister::Instance();
    // registry.registerSerializeType<ConcreteNetworkSerializable>();

    std::unique_ptr<INetworkSerializable> obj = registry.CreateSerializeInstance(GetTypeId<ConcreteNetworkSerializable>());
    ASSERT_NE(obj, nullptr);
}

class NetworkPacketTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Set up necessary objects before each test.
	}
};

TEST_F(NetworkPacketTest, DefaultConstructor)
{
	NetworkPacket packet;
	EXPECT_EQ(packet.messageID, 0);
	EXPECT_EQ(packet.networkObjectID, UINT16_MAX);
	EXPECT_EQ(packet.prefabID, INT32_MAX);
	EXPECT_EQ(packet.timestamp, 0);
	EXPECT_EQ(packet.clientGUID, SLNet::UNASSIGNED_RAKNET_GUID);
	EXPECT_EQ(packet.ISerializableID, UINT32_MAX);
	EXPECT_EQ(packet.networkBehaviourID, UINT8_MAX);
	EXPECT_EQ(packet.networkVariableID, UINT8_MAX);
}

TEST_F(NetworkPacketTest, SetTimeStampNow)
{
	NetworkPacket packet;
	packet.SetTimeStampNow();
	EXPECT_NE(packet.timestamp, 0);
}

class NetworkSharedFunctionsTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Set up necessary objects before each test.
	}
};

TEST_F(NetworkSharedFunctionsTest, MakeBitStream)
{
	SLNet::BitStream bitStream;
	NetworkSharedFunctions::reserveNetworkPacketBits(bitStream);

	NetworkPacket packet = NetworkSharedFunctions::getBitStreamData(bitStream);

	EXPECT_EQ(packet.messageID, 0);
	EXPECT_EQ(packet.networkObjectID, UINT16_MAX);
	EXPECT_EQ(packet.prefabID, INT32_MAX);
	EXPECT_EQ(packet.timestamp, 0);
	EXPECT_EQ(packet.clientGUID, SLNet::UNASSIGNED_RAKNET_GUID);
	EXPECT_EQ(packet.ISerializableID, UINT32_MAX);
	EXPECT_EQ(packet.networkBehaviourID, UINT8_MAX);
	EXPECT_EQ(packet.networkVariableID, UINT8_MAX);
}

TEST_F(NetworkSharedFunctionsTest, SetBitStreamNetworkPacket)
{
	SLNet::BitStream bitStream;
	NetworkSharedFunctions::reserveNetworkPacketBits(bitStream);
	NetworkPacket packet;
	packet.messageID = 1;
	packet.networkObjectID = 2;
	packet.prefabID = 3;
	packet.timestamp = 4;
	packet.clientGUID = SLNet::RakNetGUID(5);
	packet.ISerializableID = 6;
	packet.networkBehaviourID = 7;
	packet.networkVariableID = 8;

	NetworkSharedFunctions::setBitStreamNetworkPacket(bitStream, packet);

	NetworkPacket extractedPacket = NetworkSharedFunctions::getBitStreamData(bitStream);

	EXPECT_EQ(extractedPacket.messageID, 1);
	EXPECT_EQ(extractedPacket.networkObjectID, 2);
	EXPECT_EQ(extractedPacket.prefabID, 3);
	EXPECT_EQ(extractedPacket.timestamp, 4);
	EXPECT_EQ(extractedPacket.clientGUID, SLNet::RakNetGUID(5));
	EXPECT_EQ(extractedPacket.ISerializableID, 6);
	EXPECT_EQ(extractedPacket.networkBehaviourID, 7);
	EXPECT_EQ(extractedPacket.networkVariableID, 8);
}

class INetworkBehaviourTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		gameObject.addComponent<NetworkObject>();
		gameObject.addComponent<ConcreteNetworkBehaviour>();
		behaviour = &gameObject.getComponents<INetworkBehaviour>()[0].get();
	}

	GameObject gameObject;
	INetworkBehaviour* behaviour;
};

TEST_F(INetworkBehaviourTest, GetNetworkBehaviourID)
{
	uint8_t id = behaviour->getNetworkBehaviourID();
	EXPECT_EQ(id, 1);
}

TEST_F(INetworkBehaviourTest, ServerRpc) { EXPECT_THROW(behaviour->serverRpc(), std::runtime_error); }

TEST_F(INetworkBehaviourTest, ClientRpc) { EXPECT_THROW(behaviour->clientRpc(), std::runtime_error); }

TEST_F(INetworkBehaviourTest, OnNetworkSpawn) { EXPECT_THROW(behaviour->onNetworkSpawn(), std::runtime_error); }

TEST_F(INetworkBehaviourTest, RegisterNetworkVariable)
{
	const auto& variables = behaviour->GetNetworkVariables();
	ASSERT_EQ(variables.size(), 1);
}

TEST_F(INetworkBehaviourTest, IsOwner)
{
	NetworkObject& networkObject = gameObject.getComponents<NetworkObject>()[0];
	networkObject.setOwner(true);
	EXPECT_TRUE(behaviour->isOwner());

	networkObject.setOwner(false);
	EXPECT_FALSE(behaviour->isOwner());
}