#include "Network/NetworkObject.h"
#include "Network/INetworkBehaviour.h"

NetworkObject::NetworkObject(std::string aTag)
    : Component{aTag}, mIsOwner(false), mClientID(SLNet::UNASSIGNED_RAKNET_GUID), mIsPlayer(false),
      mNetworkBehaviours{} {}

NetworkObject::NetworkObject(const NetworkObject& other)
    : Component{other}, mIsOwner(other.mIsOwner), mClientID(other.mClientID), mIsPlayer(other.mIsPlayer),
      mNetworkBehaviours(other.mNetworkBehaviours) {}

NetworkObject& NetworkObject::operator=(const NetworkObject& other) {
    if (this != &other) {
        Component::operator=(other);
        mIsOwner = other.mIsOwner;
        mClientID = other.mClientID;
        mIsPlayer = other.mIsPlayer;
        mNetworkBehaviours = other.mNetworkBehaviours;
    }
    return *this;
}

NetworkObject::NetworkObject(NetworkObject&& other) noexcept
    : Component{std::move(other)}, mIsOwner(other.mIsOwner), mClientID(other.mClientID), mIsPlayer(other.mIsPlayer),
      mNetworkBehaviours(std::move(other.mNetworkBehaviours)) {
    other.mIsOwner = false;
    other.mClientID = SLNet::UNASSIGNED_RAKNET_GUID;
    other.mIsPlayer = false;
}

NetworkObject& NetworkObject::operator=(NetworkObject&& other) noexcept {
    if (this != &other) {
        Component::operator=(std::move(other));
        mIsOwner = other.mIsOwner;
        mClientID = other.mClientID;
        mIsPlayer = other.mIsPlayer;
        mNetworkBehaviours = std::move(other.mNetworkBehaviours);

        other.mIsOwner = false;
        other.mClientID = SLNet::UNASSIGNED_RAKNET_GUID;
        other.mIsPlayer = false;
    }
    return *this;
}

std::unique_ptr<Component> NetworkObject::clone() const { return std::make_unique<NetworkObject>(*this); }

void NetworkObject::setClientID(SLNet::RakNetGUID aClientID) { mClientID = aClientID; }

SLNet::RakNetGUID NetworkObject::getClientID() const { return mClientID; }

void NetworkObject::setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }

bool NetworkObject::isOwner() const { return mIsOwner; }

bool NetworkObject::isPlayer() const { return mIsPlayer; }

void NetworkObject::addNetworkBehaviour(INetworkBehaviour* aNetworkBehaviour) {
    mNetworkBehaviours.push_back(aNetworkBehaviour);
    aNetworkBehaviour->setOwner(mIsOwner);
}

void NetworkObject::setPlayer(bool aIsPlayer) { mIsPlayer = aIsPlayer; }