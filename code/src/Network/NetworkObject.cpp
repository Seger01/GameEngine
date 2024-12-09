/**
 * @file NetworkObject.cpp
 * @brief Implementation of the NetworkObject class.
 */

#include "Network/NetworkObject.h"
#include "Network/INetworkBehaviour.h"

int NetworkObject::networkObjectIDCounter = 0;

/**
 * @brief Constructs a new NetworkObject with a specified tag.
 * @param aTag The tag for the NetworkObject.
 */
NetworkObject::NetworkObject(std::string aTag)
	: Component{aTag}, mIsOwner(false), mClientGUID(SLNet::UNASSIGNED_RAKNET_GUID), mIsPlayer(false)
{
	mNetworkObjectID = networkObjectIDCounter++;
}

/**
 * @brief Copy constructor for NetworkObject.
 * @param other The NetworkObject to copy from.
 */
NetworkObject::NetworkObject(const NetworkObject& other)
	: Component{other}, mIsOwner(other.mIsOwner), mClientGUID(other.mClientGUID), mIsPlayer(other.mIsPlayer)
{
}

/**
 * @brief Copy assignment operator for NetworkObject.
 * @param other The NetworkObject to copy from.
 * @return A reference to this NetworkObject.
 */
NetworkObject& NetworkObject::operator=(const NetworkObject& other) {
    if (this != &other) {
        Component::operator=(other);
        mIsOwner = other.mIsOwner;
		mClientGUID = other.mClientGUID;
		mIsPlayer = other.mIsPlayer;
	}
	return *this;
}

/**
 * @brief Move constructor for NetworkObject.
 * @param other The NetworkObject to move from.
 */
NetworkObject::NetworkObject(NetworkObject&& other) noexcept
	: Component{std::move(other)}, mIsOwner(other.mIsOwner), mClientGUID(other.mClientGUID), mIsPlayer(other.mIsPlayer)
{
	other.mIsOwner = false;
	other.mClientGUID = SLNet::UNASSIGNED_RAKNET_GUID;
	other.mIsPlayer = false;
}

/**
 * @brief Move assignment operator for NetworkObject.
 * @param other The NetworkObject to move from.
 * @return A reference to this NetworkObject.
 */
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

/**
 * @brief Clones the NetworkObject.
 * @return A unique pointer to the cloned NetworkObject.
 */
std::unique_ptr<Component> NetworkObject::clone() const { return std::make_unique<NetworkObject>(*this); }

/**
 * @brief Sets the client GUID.
 * @param aClientID The client GUID to set.
 */
void NetworkObject::setClientGUID(SLNet::RakNetGUID aClientID) { mClientGUID = aClientID; }

/**
 * @brief Gets the client GUID.
 * @return The client GUID.
 */
SLNet::RakNetGUID NetworkObject::getClientGUID() const { return mClientGUID; }

/**
 * @brief Sets the owner status.
 * @param aIsOwner The owner status to set.
 */
void NetworkObject::setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }

/**
 * @brief Checks if this NetworkObject is the owner.
 * @return True if this NetworkObject is the owner, false otherwise.
 */
bool NetworkObject::isOwner() const { return mIsOwner; }

/**
 * @brief Checks if this NetworkObject is a player.
 * @return True if this NetworkObject is a player, false otherwise.
 */
bool NetworkObject::isPlayer() const { return mIsPlayer; }

/**
 * @brief Gets the network object ID.
 * @return The network object ID.
 */
int NetworkObject::getNetworkObjectID() const { return mNetworkObjectID; }

/**
 * @brief Sets the player status.
 * @param aIsPlayer The player status to set.
 */
void NetworkObject::setPlayer(bool aIsPlayer) { mIsPlayer = aIsPlayer; }

/**
 * @brief Sets the network object ID.
 * @param aNetworkObjectID The network object ID to set.
 */
void NetworkObject::setNetworkObjectID(int aNetworkObjectID) { mNetworkObjectID = aNetworkObjectID; }