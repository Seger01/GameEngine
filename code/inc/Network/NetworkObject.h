#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include "Components/Component.h"

#include <slikenet/types.h>

#include <vector>

class NetworkManager;
class INetworkBehaviour;

class NetworkObject : public Component {
public:
    NetworkObject(std::string aTag = "defaultNetworkObject");
    ~NetworkObject() = default;

    NetworkObject(const NetworkObject& other);
    NetworkObject& operator=(const NetworkObject& other);

    NetworkObject(NetworkObject&& other) noexcept;
    NetworkObject& operator=(NetworkObject&& other) noexcept;

    std::unique_ptr<Component> clone() const override;

    void setClientID(SLNet::RakNetGUID aClientID);
    SLNet::RakNetGUID getClientID() const;

    void setOwner(bool aIsOwner);
    bool isOwner() const;

    bool isPlayer() const;

    void addNetworkBehaviour(INetworkBehaviour* aNetworkBehaviour);

private:
    bool mIsOwner;
    SLNet::RakNetGUID mClientID;
    std::vector<INetworkBehaviour*> mNetworkBehaviours;

    friend class NetworkManager;
    bool mIsPlayer;
    void setPlayer(bool aIsPlayer);
};

#endif // NETWORKOBJECT_H