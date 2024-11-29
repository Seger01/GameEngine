#ifndef CONCRETENETWORKSERIALIZABLE_H
#define CONCRETENETWORKSERIALIZABLE_H

#include "Network/INetworkSerializable.h"

class ConcreteNetworkSerializable : public INetworkSerializable {
public:
    bool operator==(const ConcreteNetworkSerializable& other) const {
        return mValue == other.mValue && mFloat == other.mFloat;
    }

    bool operator!=(const ConcreteNetworkSerializable& other) const { return !(*this == other); }

    void serialize(SLNet::BitStream& stream) const override;

    void deserialize(SLNet::BitStream& stream) override;

    void setValue(int value) { mValue = value; }
    int getValue() const { return mValue; }

    void setFloat(float value) { mFloat = value; }
    float getFloat() const { return mFloat; }

private:
    int mValue = 10;
    float mFloat = 3.14f;
};

#endif // CONCRETENETWORKSERIALIZABLE_H