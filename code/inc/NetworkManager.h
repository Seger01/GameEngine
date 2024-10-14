#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <slikenet/peerinterface.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <iostream>
#include <vector>

// Simple enum to define roles
enum NetworkRole
{
    CLIENT,
    SERVER,
    HOST
};

enum GameMessages
{
    ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
    ID_PLAYER_INPUT = ID_GAME_MESSAGE_1 + 1,
    ID_PLAYER_STATE = ID_PLAYER_INPUT + 1,
    ID_PLAYER_INIT = ID_PLAYER_STATE + 1,
    ID_GAME_READY = ID_PLAYER_INIT + 1,
    ID_SEND_PLAYERS = ID_GAME_READY + 1,
    ID_SEND_PLAYER_ID = ID_SEND_PLAYERS + 1
};

class NetworkManager
{
public:
    SLNet::RakPeerInterface *mPeer;
    NetworkRole mRole;

    NetworkManager();
    ~NetworkManager();

    void StartServer(unsigned short port = 60000);
    void StartClient(const char *serverIP = "127.0.0.1", unsigned short serverPort = 60000);
    void StartHost(unsigned short port = 60000);
    void Update();

private:
    void Shutdown();
};

#endif // NETWORKMANAGER_H