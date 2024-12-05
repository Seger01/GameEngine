#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "GameObject.h"
#include <chrono>
#include <memory>
#include <slikenet/peerinterface.h>
#include <vector>

class NetworkClient {
public:
    NetworkClient(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate);
    ~NetworkClient();
    void connectToServer();
	void update();
	void discoverServers();
    std::vector<std::string>& getServerAddresses();
    void setServerAddress(std::string aServerAddress);
    bool isConnected() const;

private:
    void sendPackets();
    void requestPlayerInstantiation();
    void handleIncomingPackets();
    void sendTransform();
    void handleTransform(SLNet::Packet* aPacket);
    void handlePlayerInstantiation(SLNet::Packet* aPacket);
    void handlePlayerDestruction(SLNet::Packet* aPacket);
	void sendCustomSerialize();
	void handleCustomSerialize(SLNet::Packet* aPacket);
	void sendToServer(SLNet::BitStream& aBitStream);

private:
    bool mIsConnected;
    bool mIsConnecting;
    std::unique_ptr<SLNet::RakPeerInterface, void (*)(SLNet::RakPeerInterface*)> mClient;
    std::vector<std::string> mServerAddresses;
    std::string mServerAddress;
    SLNet::RakNetGUID mServerGUID;

    std::vector<std::reference_wrapper<GameObject>>& mObjects;
    int mTickRate;
    std::chrono::time_point<std::chrono::steady_clock> mLastSendPacketsTime;
};

#endif // NETWORKCLIENT_H