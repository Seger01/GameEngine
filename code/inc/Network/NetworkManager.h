#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "GameObject.h"
#include "Network/NetworkClient.h"
#include "Network/NetworkHost.h"
#include "Network/NetworkInformation.h"
#include "Network/NetworkServer.h"
#include <memory>
#include <stdexcept>

class NetworkManager {
public:
    NetworkManager();

    void startNetwork();
    void shutdown();

    void initialize();
    void update();

    NetworkServer& getServer() const;
    NetworkClient& getClient() const;
    NetworkHost& getHost() const;

    bool isServer() const;
    bool isClient() const;
    bool isHost() const;
    bool isConnected() const;

    void setTickRate(int aTickRate);
    int getTickRate() const;

    void setEnableSceneManagement(bool aEnableSceneManagement);
    bool getEnableSceneManagement() const;

    void setDefaultPlayerPrefab(GameObject* aDefaultPlayerPrefab);
    GameObject& getDefaultPlayerPrefab() const;
    GameObject* instantiatePlayer(SLNet::RakNetGUID playerID);
    void destroyPlayer(SLNet::RakNetGUID playerID);

    void setRole(NetworkRole aRole);
    NetworkRole getRole() const;

    std::vector<GameObject*>& getGameObjects();

private:
    void startServer();
    void startClient();
    void startHost();

private:
    NetworkRole mRole;
    int mTickRate;
    std::unique_ptr<GameObject> mDefaultPlayerPrefab;
    bool mEnableSceneManagement;
    std::vector<GameObject*>* mGameObjects;

    std::unique_ptr<NetworkServer> mServer;
    std::unique_ptr<NetworkClient> mClient;
    std::unique_ptr<NetworkHost> mHost;
};

#endif // NETWORKMANAGER_H