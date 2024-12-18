/**
 * @file NetworkObject.h
 * @brief Header file for the NetworkObject class.
 */

#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include "Components/Component.h"
#include <slikenet/types.h>
#include <vector>

class NetworkManager;

/**
 * @class NetworkObject
 * @brief Represents a networked object in the game engine.
 */
class NetworkObject : public Component {
public:
    NetworkObject(std::string aTag = "defaultNetworkObject");
	~NetworkObject() = default;
	NetworkObject(const NetworkObject& other);
	NetworkObject& operator=(const NetworkObject& other);
	NetworkObject(NetworkObject&& other) noexcept;
	NetworkObject& operator=(NetworkObject&& other) noexcept;
	std::unique_ptr<Component> clone() const override;
	void setClientGUID(SLNet::RakNetGUID aClientID);
	SLNet::RakNetGUID getClientGUID() const;
	void setOwner(bool aIsOwner);
	bool isOwner() const;
	bool isPlayer() const;
	int getNetworkObjectID() const;
	int getPrefabID() const;
	void spawn();
	void despawn();

private:
	void setPlayer(bool aIsPlayer);
	void setNetworkObjectID(int aNetworkObjectID);
	void setPrefabID(int aPrefabID);

private:
	bool mIsOwner;				   ///< Indicates if this object is the owner.
	int mNetworkObjectID;		   ///< The network object ID.
	SLNet::RakNetGUID mClientGUID; ///< The client GUID.

	friend class NetworkManager;
	bool mIsPlayer; ///< Indicates if this object is a player.

	int mPrefabID; ///< The prefab ID of the object.

	static int networkObjectIDCounter; ///< Counter for generating unique network object IDs.
};

#endif // NETWORKOBJECT_H