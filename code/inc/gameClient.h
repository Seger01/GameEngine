#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "gameBase.h"

class GameClient : public GameBase

{
public:
    GameClient(SLNet::RakPeerInterface *peer);
    bool init() override;

protected:
    void handleEvents() override;
    void handleNetwork() override;
};
#endif // GAME_SERVER_H