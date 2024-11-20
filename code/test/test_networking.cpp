#include "Network/INetworkSerializable.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkTransform.h"
#include <gtest/gtest.h>

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
    EXPECT_EQ(no.getClientID(), SLNet::UNASSIGNED_RAKNET_GUID);
}

TEST_F(NetworkObjectTest, SetAndGetClientID) {
    NetworkObject no;
    SLNet::RakNetGUID clientID = SLNet::RakNetGUID(12345);
    no.setClientID(clientID);
    EXPECT_EQ(no.getClientID(), clientID);
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
    no.setClientID(clientID);

    std::unique_ptr<Component> clone = no.clone();
    NetworkObject* clonedNo = dynamic_cast<NetworkObject*>(clone.get());

    ASSERT_NE(clonedNo, nullptr);
    EXPECT_TRUE(clonedNo->isOwner());
    EXPECT_EQ(clonedNo->getClientID(), clientID);
}

class TestSerializable : public INetworkSerializable {
public:
    void Serialize(SLNet::BitStream& stream) const override {
        // Serialization logic
    }

    void Deserialize(SLNet::BitStream& stream) override {
        // Deserialization logic
    }

    uint32_t GetTypeID() const override { return TYPE_ID(TestSerializable); }

    AUTO_REGISTER_TYPE(TestSerializable)
};

TEST_F(NetworkObjectTest, TestSerializableRegistration) {
    auto& registry = NetworkRegister::Instance();
    // registry.RegisterType<TestSerializable>();

    std::unique_ptr<INetworkSerializable> obj = registry.Create(TYPE_ID(TestSerializable));
    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->GetTypeID(), TYPE_ID(TestSerializable));
}