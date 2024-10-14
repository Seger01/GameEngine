#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "NetworkManager.h"

#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>

class NetworkClient
{
public:
    void Connect(NetworkManager &manager, const char *serverIP = "127.0.0.1", unsigned short port = 60000);
    void SendInput(SLNet::RakPeerInterface *peer, float inputX, float inputY);
};

#endif // NETWORKCLIENT_H