#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include "Components/Component.h"

class NetworkObject : public Component {
public:
    void spawn();
    void despawn();
    int getNetworkID();

private:
    bool mIsOwner;
    int mNetworkID;
};

#endif // NETWORKOBJECT_H