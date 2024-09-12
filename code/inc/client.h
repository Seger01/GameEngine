#ifndef CLIENT_H
#define CLIENT_H

#include <slikenet/peerinterface.h>

class Client
{
public:
    Client();

    void init();
    void run();
    void cleanup();

private:
    SLNet::RakPeerInterface *peer;
};

#endif // CLIENT_H