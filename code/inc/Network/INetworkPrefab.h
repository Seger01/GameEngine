/**
 * @file INetworkPrefab.h
 * @brief Header file for the INetworkPrefab class.
 */
#ifndef INETWORKENEMYPREFAB_H
#define INETWORKENEMYPREFAB_H

#include "GameObject.h"
#include "Network/NetworkRegister.h"

/**
 * @class INetworkPrefab
 * @brief Interface for network enemy prefabs.
 * @note when making a derived class always use REGISTER_NETWORK_PREFAB(Derived) in the cpp file.
 */
class INetworkPrefab
{
public:
	INetworkPrefab() = default;
	virtual ~INetworkPrefab() = default;

	/**
	 * @brief Creates a new GameObject instance.
	 */
	virtual GameObject* create() = 0;

	/**
	 * @brief Gets the prefab ID.
	 * @note This function should always be: return GetTypeId<Derived>(); where Derived is the derived class.
	 * @return The prefab ID.
	 */
	virtual int getPrefabID() const = 0;
};

#endif // INETWORKENEMYPREFAB_H