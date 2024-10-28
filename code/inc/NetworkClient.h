#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <memory>
#include <slikenet/peerinterface.h>

class NetworkClient
{
public:
    NetworkClient();
    void connectToServer();
    void sendGameState();
    void receiveGameState();
    void update();

private:
    bool mIsConnected;
    bool mIsConnecting;
    int mClientID;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface *)> mPeer;
    static constexpr const char *SERVER_ADDRESS = "192.168.1.100";
    static constexpr unsigned short SERVER_PORT = 60001;
    static constexpr unsigned short CLIENT_PORT = 60002;
};

#endif // NETWORKCLIENT_H