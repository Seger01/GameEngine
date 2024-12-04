#include "Network/INetworkBehaviour.h"
#include "Network/INetworkSerializable.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkRegister.h"
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
    // registry.RegisterType<ConcreteNetworkSerializable>();

    std::unique_ptr<INetworkSerializable> obj = registry.CreateInstance(GetTypeId<ConcreteNetworkSerializable>());
    ASSERT_NE(obj, nullptr);
}
