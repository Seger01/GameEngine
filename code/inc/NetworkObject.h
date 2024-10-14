#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <slikenet/PacketPriority.h>

class NetworkObject
{
public:
    SLNet::RakNetGUID mOwner;
    unsigned int mNetworkID;

    NetworkObject();

    // Sync position over network
    void SyncState(SLNet::RakPeerInterface *peer, SLNet::SystemAddress targetAddress);

    // Update position based on received network data
    void UpdateState(SLNet::BitStream &bs);
};

#endif // NETWORKOBJECT_H