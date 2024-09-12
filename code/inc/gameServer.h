#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "gameBase.h"

#include <map>

class GameServer : public GameBase
{
public:
    GameServer(SLNet::RakPeerInterface *peer);

protected:
    void handleEvents() override;
    void handleNetwork() override;

protected:
    std::map<SLNet::SystemAddress, int> clientIds;
    int nextClientId;
};
#endif // GAME_SERVER_H