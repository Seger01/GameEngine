#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <slikenet/PacketPriority.h>

class NetworkBehaviour
{
public:
    virtual void SyncState(SLNet::RakPeerInterface *peer, SLNet::SystemAddress targetAddress) = 0;
    virtual void UpdateState(SLNet::BitStream &bs) = 0;
};

#endif // NETWORKBEHAVIOUR_H