#ifndef NETWORKREGISTER_H
#define NETWORKREGISTER_H

#include "Network/INetworkSerializable.h"
#include "Network/NetworkInformation.h"

#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>

// Compile-time hash function for type names
constexpr uint32_t HashString(const char* str, std::size_t length) {
    uint32_t hash = 2166136261u;
    for (std::size_t i = 0; i < length; ++i) {
        hash ^= static_cast<uint32_t>(str[i]);
        hash *= 16777619u;
    }
    return hash;
}

// Compile-time type name resolution
template <typename T> constexpr std::string_view GetTypeName() {
    return "UnknownType"; // Fallback for unsupported types
}

// Specialization for templates
template <typename T> constexpr std::string_view GetTypeNameTemplate() { return GetTypeName<T>(); }

template <typename T> constexpr uint32_t TYPE_ID() {
    return HashString(GetTypeNameTemplate<T>().data(), GetTypeNameTemplate<T>().size());
}

// Macro to generate a consistent type ID for a given type
// template <typename T> constexpr uint32_t TYPE_ID() {
//     constexpr auto typeName = GetTypeName<T>();
//     return HashString(typeName.data(), typeName.size());
// }

class INetworkSerializable;
class NetworkRegister {
public:
    using CreateFunction = std::function<std::unique_ptr<INetworkSerializable>()>;

    static NetworkRegister& Instance() {
        static NetworkRegister instance;
        return instance;
    }

    // Register a type with its creation function
    template <typename T> void RegisterType() {
        uint32_t typeID = TYPE_ID<T>();
        registry[typeID] = []() { return std::make_unique<T>(); };
    }

    // Create an instance of the type corresponding to the typeID
    std::unique_ptr<INetworkSerializable> Create(uint32_t typeID) const {
        auto it = registry.find(typeID);
        if (it != registry.end()) {
            return it->second();
        }
        return nullptr; // Type not registered
    }

private:
    std::unordered_map<uint32_t, CreateFunction> registry; // Maps type IDs to creation functions
};

#endif // NETWORKREGISTER_H
