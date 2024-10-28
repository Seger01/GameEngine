#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

class NetworkBehaviour
{
public:
    void serverRpc();
    void clientRpc();
    virtual void OnNetworkSpawn() = 0;
};

#endif // NETWORKBEHAVIOUR_H