/**
 * @file NetworkHost.h
 * @brief The file contains the declaration of the NetworkHost class.
 */
#ifndef NETWORKHOST_H
#define NETWORKHOST_H

#include "Network/NetworkClient.h"
#include "Network/NetworkServer.h"

#include <memory>
#include <vector>

class GameObject;

/**
 * @brief The NetworkHost class manages both the server and client for network communication.
 */
class NetworkHost {
public:
	NetworkHost(std::vector<std::reference_wrapper<GameObject>>& aObjects, int aTickRate = 60);

	void update();

private:
	std::unique_ptr<NetworkServer> mServer; /**< The server for the network host. */
	std::unique_ptr<NetworkClient> mClient; /**< The client for the network host. */
	;
};

#endif // NETWORKHOST_H