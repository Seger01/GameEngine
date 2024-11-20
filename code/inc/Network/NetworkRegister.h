#ifndef NETWORKREGISTER_H
#define NETWORKREGISTER_H

#include "Network/INetworkSerializable.h"
#include "Network/NetworkInformation.h"
#include "Network/NetworkTypeIDGenerator.h"

#include <functional>
#include <memory>
#include <unordered_map>

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
        uint32_t typeID = TYPE_ID(T);
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
