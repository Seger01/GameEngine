#include "Network/INetworkBehaviour.h"
#include "Network/INetworkSerializable.h"
#include "Network/NetworkClient.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkRegister.h"
#include "Network/NetworkServer.h"
#include "Network/NetworkSharedFunctions.h"
#include "Network/NetworkTransform.h"
#include <gtest/gtest.h>

#include "ConcreteNetworkBehaviour.h"
#include "ConcreteNetworkSerializable.h"

class NetworkTransformTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up necessary objects before each test.
    }
};

TEST_F(NetworkTransformTest, DefaultConstructor) {
    NetworkTransform nt;
    EXPECT_FALSE(nt.getSendPositionX());
    EXPECT_FALSE(nt.getSendPositionY());
    EXPECT_FALSE(nt.getSendRotation());
    EXPECT_FALSE(nt.getSendScaleX());
    EXPECT_FALSE(nt.getSendScaleY());
}

TEST_F(NetworkTransformTest, ParameterizedConstructor) {
    NetworkTransform nt(true, true, true, true, true);
    EXPECT_TRUE(nt.getSendPositionX());
    EXPECT_TRUE(nt.getSendPositionY());
    EXPECT_TRUE(nt.getSendRotation());
    EXPECT_TRUE(nt.getSendScaleX());
    EXPECT_TRUE(nt.getSendScaleY());
}

TEST_F(NetworkTransformTest, SettersAndGetters) {
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

TEST_F(NetworkTransformTest, Clone) {
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

class NetworkObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up necessary objects before each test.
    }
};

TEST_F(NetworkObjectTest, DefaultConstructor) {
    NetworkObject no;
    EXPECT_FALSE(no.isOwner());
	EXPECT_EQ(no.getClientGUID(), SLNet::UNASSIGNED_RAKNET_GUID);
}

TEST_F(NetworkObjectTest, SetAndGetClientID) {
    NetworkObject no;
    SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
	no.setClientGUID(clientID);
	EXPECT_EQ(no.getClientGUID(), clientID);
}

TEST_F(NetworkObjectTest, SetAndGetOwner) {
    NetworkObject no;
    no.setOwner(true);
    EXPECT_TRUE(no.isOwner());
    no.setOwner(false);
    EXPECT_FALSE(no.isOwner());
}

TEST_F(NetworkObjectTest, Clone) {
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

class NetworkBehaviourTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(NetworkBehaviourTest, RegisterNetworkVariable) {
    GameObject gameObject;
    gameObject.addComponent<NetworkObject>();
    gameObject.addComponent<ConcreteNetworkBehaviour>();

    INetworkBehaviour* behaviour = gameObject.getComponents<INetworkBehaviour>()[0];

    const auto& variables = behaviour->GetNetworkVariables();
    ASSERT_EQ(variables.size(), 1);
}

class INetworkSerializableTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(INetworkSerializableTest, TestSerializableRegistration) {
    auto& registry = NetworkRegister::Instance();
	registry.RegisterType<ConcreteNetworkSerializable>();

	std::unique_ptr<INetworkSerializable> obj = registry.CreateInstance(GetTypeId<ConcreteNetworkSerializable>());
    ASSERT_NE(obj, nullptr);
}

class NetworkInformationTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Set up necessary objects before each test.
	}
};

TEST_F(NetworkInformationTest, DefaultConstructor)
{
	NetworkPacket packet;
	EXPECT_EQ(packet.messageID, 0);
	EXPECT_EQ(packet.networkObjectID, UINT16_MAX);
	EXPECT_EQ(packet.prefabID, UINT16_MAX);
	EXPECT_EQ(packet.timestamp, 0);
	EXPECT_EQ(packet.clientGUID, SLNet::UNASSIGNED_RAKNET_GUID);
	EXPECT_EQ(packet.ISerializableID, UINT16_MAX);
	EXPECT_EQ(packet.networkBehaviourID, UINT8_MAX);
	EXPECT_EQ(packet.networkVariableID, UINT8_MAX);
}

TEST_F(NetworkInformationTest, SetTimeStampNow)
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
	NetworkSharedFunctions::reserverNetworkPacketBits(bitStream);

	NetworkPacket packet = NetworkSharedFunctions::getBitStreamData(bitStream);

	EXPECT_EQ(packet.messageID, 0);
	EXPECT_EQ(packet.networkObjectID, UINT16_MAX);
	EXPECT_EQ(packet.prefabID, UINT16_MAX);
	EXPECT_EQ(packet.timestamp, 0);
	EXPECT_EQ(packet.clientGUID, SLNet::UNASSIGNED_RAKNET_GUID);
	EXPECT_EQ(packet.ISerializableID, UINT16_MAX);
	EXPECT_EQ(packet.networkBehaviourID, UINT8_MAX);
	EXPECT_EQ(packet.networkVariableID, UINT8_MAX);
}

TEST_F(NetworkSharedFunctionsTest, SetBitStreamNetworkPacket)
{
	SLNet::BitStream bitStream;
	NetworkSharedFunctions::reserverNetworkPacketBits(bitStream);
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

// class NetworkTest : public ::testing::Test
// {
// protected:
// 	void SetUp() override
// 	{
// 		// Set up necessary objects before each test.
// 		serverObjects = std::vector<std::reference_wrapper<GameObject>>();
// 		clientObjects = std::vector<std::reference_wrapper<GameObject>>();

// 		server = std::make_unique<NetworkServer>(serverObjects, 60);
// 		client = std::make_unique<NetworkClient>(clientObjects, 60);
// 	}

// 	void TearDown() override
// 	{
// 		// Clean up after each test.
// 		server.reset();
// 		client.reset();
// 	}

// 	std::unique_ptr<NetworkServer> server;
// 	std::unique_ptr<NetworkClient> client;
// 	std::vector<std::reference_wrapper<GameObject>> serverObjects;
// 	std::vector<std::reference_wrapper<GameObject>> clientObjects;
// };

// TEST_F(NetworkTest, ServerClientConnection)
// {
// 	// Set server address for the client
// 	client->setServerAddress("127.0.0.1");

// 	// Connect client to server
// 	client->connectToServer();

// 	// Simulate server and client update loops
// 	for (int i = 0; i < 100; ++i)
// 	{
// 		server->update();
// 		client->update();
// 	}

// 	// Verify that the client is connected to the server
// 	EXPECT_TRUE(client->isConnected());
// 	EXPECT_TRUE(server->isConnected());
// }
