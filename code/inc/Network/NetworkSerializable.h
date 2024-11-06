#ifndef INETWORKSERIALIZABLE_H
#define INETWORKSERIALIZABLE_H

class INetworkSerializable
{
public:
    virtual void networkSerialize() = 0;
};

#endif // INETWORKSERIALIZABLE_H