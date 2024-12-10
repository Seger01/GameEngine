/**
 * @file NetworkRegister.h
 * @brief Header file for the NetworkRegister class and related functions.
 */

#ifndef NETWORKREGISTER_H
#define NETWORKREGISTER_H

#include "Network/INetworkPrefab.h"
#include "Network/INetworkSerializable.h"
#include "Network/NetworkInformation.h"

#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

class INetworkSerializable;

/**
 * @brief A simple compile-time hash function (FNV-1a Hash).
 * @param str The input string to hash.
 * @param length The length of the input string.
 * @return The computed hash value.
 */
constexpr std::uint32_t CompileTimeHash(const char* str, std::size_t length)
{
	std::uint32_t hash = 2166136261u;
	for (std::size_t i = 0; i < length; ++i)
	{
		hash ^= static_cast<std::uint32_t>(str[i]);
		hash *= 16777619u;
	}
	return hash;
}

/**
 * @brief Gets the compile-time type ID for a given type.
 * @tparam T The type for which to get the type ID.
 * @return The compile-time type ID.
 */
template <typename T> constexpr std::uint32_t GetCompileTimeTypeId()
{
	// Use the type name as a unique key; ensure strlen instead of sizeof
	return CompileTimeHash(typeid(T).name(), std::strlen(typeid(T).name()));
}

/**
 * @brief Gets the type ID for a given type.
 * @tparam T The type for which to get the type ID.
 * @return The type ID.
 */
template <typename T> std::uint32_t GetTypeId() { return GetCompileTimeTypeId<T>(); }

/**
 * @brief Factory function type for creating instances of INetworkSerializable.
 */
using NetworkSerializableFactory = std::function<std::unique_ptr<INetworkSerializable>()>;

/**
 * @brief Factory function type for creating instances of INetworkPrefab.
 */
using NetworkPrefabFactory = std::function<std::unique_ptr<INetworkPrefab>()>;

/**
 * @brief A registry for network serializable types.
 */
class NetworkRegister
{
public:
	/**
	 * @brief Gets the singleton instance of the NetworkRegister.
	 * @return The singleton instance.
	 */
	static NetworkRegister& Instance()
	{
		static NetworkRegister instance;
		return instance;
	}

	/**
	 * @brief Registers a Serialize type with a compile-time ID.
	 * @tparam T The type to register.
	 */
	template <typename T> void registerSerializeType()
	{
		static_assert(std::is_base_of<INetworkSerializable, T>::value, "Type must derive from INetworkSerializable");
		std::uint32_t typeId = GetCompileTimeTypeId<T>();
		std::cout << "Registering type: " << typeId << std::endl;
		mSerializeRegistry[typeId] = []() { return std::make_unique<T>(); };
	}

	/**
	 * @brief Registers an EnemyPrefab type with a compile-time ID.
	 * @tparam T The type to register.
	 */
	template <typename T> void registerPrefabType()
	{
		static_assert(std::is_base_of<INetworkPrefab, T>::value, "Type must derive from INetworkPrefab");
		std::uint32_t typeId = GetCompileTimeTypeId<T>();
		std::cout << "Registering enemy type: " << typeId << std::endl;
		mPrefabRegistry[typeId] = []() { return std::make_unique<T>(); };
	}

	/**
	 * @brief Creates an Serialize instance based on the compile-time type ID.
	 * @param typeId The compile-time type ID.
	 * @return A unique pointer to the created instance, or nullptr if the type ID is not registered.
	 */
	std::unique_ptr<INetworkSerializable> CreateSerializeInstance(std::uint32_t typeId)
	{
		if (mSerializeRegistry.find(typeId) != mSerializeRegistry.end())
		{
			return mSerializeRegistry[typeId]();
		}
		return nullptr;
	}

	/**
	 * @brief Creates an Prefab instance based on the compile-time type ID.
	 * @param typeId The compile-time type ID.
	 * @return A unique pointer to the created instance, or nullptr if the type ID is not registered.
	 */
	std::unique_ptr<INetworkPrefab> CreatePrefabInstance(std::uint32_t typeId)
	{
		if (mPrefabRegistry.find(typeId) != mPrefabRegistry.end())
		{
			return mPrefabRegistry[typeId]();
		}
		return nullptr;
	}

private:
	/**
	 * @brief The registry mapping type IDs to factory functions.
	 */
	std::unordered_map<std::uint32_t, NetworkSerializableFactory> mSerializeRegistry;

	/**
	 * @brief The registry mapping enemy IDs to factory functions.
	 */
	std::unordered_map<std::uint32_t, NetworkPrefabFactory> mPrefabRegistry;
};

/**
 * @brief Macro to register a network serializable type.
 * @param T The type to register.
 */
#define REGISTER_NETWORK_SERIALIZABLE(T)                                                                               \
	static bool T##_registered = []                                                                                    \
	{                                                                                                                  \
		NetworkRegister::Instance().registerSerializeType<T>();                                                        \
		return true;                                                                                                   \
	}()

#define REGISTER_NETWORK_ENEMY(T)                                                                                      \
	static bool T##_registered = []                                                                                    \
	{                                                                                                                  \
		NetworkRegister::Instance().registerPrefabType<T>();                                                           \
		return true;                                                                                                   \
	}()
#endif // NETWORKREGISTER_H
