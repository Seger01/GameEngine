#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

class NetworkObject
{
public:
    void spawn();
    void despawn();
    int getNetworkID();

private:
    bool mIsOwner;
    int mNetworkID;
};

#endif // NETWORKOBJECT_H