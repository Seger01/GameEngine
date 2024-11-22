#ifndef NETWORKVARIABLE_H
#define NETWORKVARIABLE_H

#include "Network/NetworkInformation.h"

class INetworkBehaviour;
class NetworkVariableBase {
public:
    virtual ~NetworkVariableBase() = default;

    virtual bool isDirty() const = 0;
    virtual void setClean() = 0;
};

template <typename INetworkSerializableTemplate> class NetworkVariable : public NetworkVariableBase {
public:
    // NetworkVariable(WritePermission aWritePermission);
    NetworkVariable(INetworkBehaviour* aOwner, INetworkSerializableTemplate aValue = INetworkSerializableTemplate())
        : mValue(std::move(aValue)), mDirty(false) {
        if (aOwner) {
            aOwner->RegisterNetworkVariable(this);
        } else {
            throw std::runtime_error("NetworkVariable::NetworkVariable() aOwner is nullptr");
        }
    }

    INetworkSerializableTemplate getValue() const { return mValue; }

    void setValue(INetworkSerializableTemplate aValue) {
        if (mValue != aValue) {
            mValue = aValue;
            mDirty = true;
        }
    }

    bool isDirty() const override { return mDirty; }

    void setClean() override { mDirty = false; }

private:
    INetworkSerializableTemplate mValue;
    bool mDirty;
};

#endif // NETWORKVARIABLE_H