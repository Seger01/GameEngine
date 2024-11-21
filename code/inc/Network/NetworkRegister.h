#ifndef NETWORKREGISTER_H
#define NETWORKREGISTER_H

#include "Network/INetworkSerializable.h"
#include "Network/NetworkInformation.h"
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

class INetworkSerializable;

// A simple compile-time hash function (FNV-1a Hash)
constexpr std::uint32_t CompileTimeHash(const char* str, std::size_t length) {
    std::uint32_t hash = 2166136261u;
    for (std::size_t i = 0; i < length; ++i) {
        hash ^= static_cast<std::uint32_t>(str[i]);
        hash *= 16777619u;
    }
    return hash;
}

template <typename T> constexpr std::uint32_t GetCompileTimeTypeId() {
    // Use the type name as a unique key; ensure strlen instead of sizeof
    return CompileTimeHash(typeid(T).name(), std::strlen(typeid(T).name()));
}

template <typename T> std::uint32_t GetTypeId() { return GetCompileTimeTypeId<T>(); }

// Factory function type for creating instances of INetworkSerializable
using NetworkSerializableFactory = std::function<std::unique_ptr<INetworkSerializable>()>;

class NetworkRegister {
public:
    static NetworkRegister& Instance() {
        static NetworkRegister instance;
        return instance;
    }

    // Registers a type with a compile-time ID
    template <typename T> void RegisterType() {
        std::uint32_t typeId = GetCompileTimeTypeId<T>();
        registry[typeId] = []() { return std::make_unique<T>(); };
    }

    // Creates an instance based on the compile-time type ID
    std::unique_ptr<INetworkSerializable> CreateInstance(std::uint32_t typeId) {
        if (registry.find(typeId) != registry.end()) {
            return registry[typeId]();
        }
        return nullptr;
    }

private:
    std::unordered_map<std::uint32_t, NetworkSerializableFactory> registry;
};

#define REGISTER_NETWORK_SERIALIZABLE(T)                                                                               \
    static bool T##_registered = [] {                                                                                  \
        NetworkRegister::Instance().RegisterType<T>();                                                                 \
        return true;                                                                                                   \
    }()

#endif // NETWORKREGISTER_H
