#include "Network/NetworkObject.h"
#include "Network/INetworkBehaviour.h"

int NetworkObject::networkObjectIDCounter = 0;

NetworkObject::NetworkObject(std::string aTag)
	: Component{aTag}, mIsOwner(false), mClientGUID(SLNet::UNASSIGNED_RAKNET_GUID), mIsPlayer(false)
{
	mNetworkObjectID = networkObjectIDCounter++;
}

NetworkObject::NetworkObject(const NetworkObject& other)
	: Component{other}, mIsOwner(other.mIsOwner), mClientGUID(other.mClientGUID), mIsPlayer(other.mIsPlayer)
{
}

NetworkObject& NetworkObject::operator=(const NetworkObject& other) {
    if (this != &other) {
        Component::operator=(other);
        mIsOwner = other.mIsOwner;
		mClientGUID = other.mClientGUID;
		mIsPlayer = other.mIsPlayer;
	}
	return *this;
}

NetworkObject::NetworkObject(NetworkObject&& other) noexcept
	: Component{std::move(other)}, mIsOwner(other.mIsOwner), mClientGUID(other.mClientGUID), mIsPlayer(other.mIsPlayer)
{
	other.mIsOwner = false;
	other.mClientGUID = SLNet::UNASSIGNED_RAKNET_GUID;
	other.mIsPlayer = false;
}

NetworkObject& NetworkObject::operator=(NetworkObject&& other) noexcept {
    if (this != &other) {
        Component::operator=(std::move(other));
        mIsOwner = other.mIsOwner;
		mClientGUID = other.mClientGUID;
		mIsPlayer = other.mIsPlayer;

		other.mIsOwner = false;
		other.mClientGUID = SLNet::UNASSIGNED_RAKNET_GUID;
		other.mIsPlayer = false;
	}
	return *this;
}

std::unique_ptr<Component> NetworkObject::clone() const { return std::make_unique<NetworkObject>(*this); }

void NetworkObject::setClientGUID(SLNet::RakNetGUID aClientID) { mClientGUID = aClientID; }

SLNet::RakNetGUID NetworkObject::getClientGUID() const { return mClientGUID; }

void NetworkObject::setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }

bool NetworkObject::isOwner() const { return mIsOwner; }

bool NetworkObject::isPlayer() const { return mIsPlayer; }

int NetworkObject::getNetworkObjectID() const { return mNetworkObjectID; }

void NetworkObject::setPlayer(bool aIsPlayer) { mIsPlayer = aIsPlayer; }

void NetworkObject::setNetworkObjectID(int aNetworkObjectID) { mNetworkObjectID = aNetworkObjectID; }