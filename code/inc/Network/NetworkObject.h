#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include "Components/Component.h"

#include <slikenet/types.h>

class NetworkObject : public Component {
public:
    NetworkObject();

    std::unique_ptr<Component> clone() const override;

    void setClientID(SLNet::RakNetGUID aClientID);
    SLNet::RakNetGUID getClientID() const;

    void setOwner(bool aIsOwner);
    bool isOwner() const;

private:
    bool mIsOwner;
    SLNet::RakNetGUID mClientID;
};

#endif // NETWORKOBJECT_H