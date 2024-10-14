#ifndef NETWORKTRANSFORM_H
#define NETWORKTRANSFORM_H

#include "NetworkBehaviour.h"

class NetworkTransform : public NetworkBehaviour
{
public:
    void UpdateTransform();
    void SyncPosition();
    void Interpolate();

    void SyncState(SLNet::RakPeerInterface *peer, SLNet::SystemAddress targetAddress) override;
    void UpdateState(SLNet::BitStream &bs) override;

private:
    float mX, mY;
};

#endif // NETWORKTRANSFORM_H