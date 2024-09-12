#ifndef SERVER_H
#define SERVER_H

#include <slikenet/peerinterface.h>

class Server
{
public:
    Server();
    void init();
    void run();
    void cleanup();

private:
    SLNet::RakPeerInterface *peer;
};

#endif // SERVER_H