// #include "NetworkManager.h"

// NetworkManager::NetworkManager() : mPeer(SLNet::RakPeerInterface::GetInstance()), mRole(CLIENT) {}

// NetworkManager::~NetworkManager()
// {
//     Shutdown();
// }

// void NetworkManager::StartServer(unsigned short port)
// {
//     SLNet::SocketDescriptor sd(port, 0);
//     mPeer->Startup(10, &sd, 1);
//     mPeer->SetMaximumIncomingConnections(10);
//     mRole = SERVER;
//     std::cout << "Server started on port " << port << std::endl;
// }

// void NetworkManager::StartClient(const char *serverIP, unsigned short serverPort)
// {
//     SLNet::SocketDescriptor sd;
//     mPeer->Startup(1, &sd, 1);
//     mPeer->Connect(serverIP, serverPort, nullptr, 0);
//     mRole = CLIENT;
//     std::cout << "Client started and connecting to server: " << serverIP << std::endl;
// }

// void NetworkManager::StartHost(unsigned short port)
// {
//     StartServer(port);
//     StartClient("127.0.0.1", port);
//     mRole = HOST;
//     std::cout << "Host started on port " << port << std::endl;
// }

// void NetworkManager::Shutdown()
// {
//     mPeer->Shutdown(300);
//     SLNet::RakPeerInterface::DestroyInstance(mPeer);
//     std::cout << "Network shutdown complete." << std::endl;
// }

// void NetworkManager::Update()
// {
//     SLNet::Packet *packet;
//     for (packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
//     {
//         switch (packet->data[0])
//         {
//         case ID_NEW_INCOMING_CONNECTION:
//             std::cout << "A connection is incoming.\n";
//             break;
//         case ID_CONNECTION_REQUEST_ACCEPTED:
//             std::cout << "Our connection request has been accepted.\n";
//             break;
//         case ID_NO_FREE_INCOMING_CONNECTIONS:
//             std::cout << "The server is full.\n";
//             break;
//         case ID_DISCONNECTION_NOTIFICATION:
//             std::cout << "A client has disconnected.\n";
//             break;
//         case ID_CONNECTION_LOST:
//             std::cout << "A client lost the connection.\n";
//             break;
//         default:
//             std::cout << "Received packet id: " << (int)packet->data[0] << std::endl;
//             break;
//         }
//     }
// }

#include "Network/NetworkManager.h"

NetworkManager::NetworkManager() : mRole(NetworkRole::UNASSIGNED), mTickRate(60), mEnableSceneManagement(false) {}

void NetworkManager::startNetwork() {
    if (mRole == NetworkRole::SERVER) {
        startServer();
    } else if (mRole == NetworkRole::CLIENT) {
        startClient();
    } else if (mRole == NetworkRole::HOST) {
        startHost();
    }
}

void NetworkManager::shutdown() { throw std::runtime_error("NetworkManager::shutdown() not implemented"); }

void NetworkManager::initialize() { startNetwork(); }

void NetworkManager::update() {
    if (mRole == NetworkRole::SERVER && mServer) {
        mServer->update(mGameObjects);
    } else if (mRole == NetworkRole::CLIENT && mClient) {
        mClient->update(mGameObjects);
    } else if (mRole == NetworkRole::HOST && mHost) {
        throw std::runtime_error("NetworkManager::update() isHost not implemented");
        // mHost->update();
    }
}

NetworkServer& NetworkManager::getServer() const {
    if (!mServer) {
        throw std::runtime_error("Server is not running");
    }
    return *mServer;
}

NetworkClient& NetworkManager::getClient() const {
    if (!mClient) {
        throw std::runtime_error("Client is not running");
    }
    return *mClient;
}

NetworkHost& NetworkManager::getHost() const {
    if (!mHost) {
        throw std::runtime_error("Host is not running");
    }
    return *mHost;
}

bool NetworkManager::isServer() const { return mRole == NetworkRole::SERVER; }

bool NetworkManager::isClient() const { return mRole == NetworkRole::CLIENT; }

bool NetworkManager::isHost() const { return mRole == NetworkRole::HOST; }

bool NetworkManager::isConnected() const {
    if (mRole == NetworkRole::SERVER) {
        return mServer->isConnected();
    } else if (mRole == NetworkRole::CLIENT) {
        return mClient->isConnected();
    } else if (mRole == NetworkRole::HOST) {
        throw std::runtime_error("NetworkManager::isConnected() isHost not implemented");
    }
    return false;
}

void NetworkManager::setTickRate(int aTickRate) { mTickRate = aTickRate; }

int NetworkManager::getTickRate() const { return mTickRate; }

void NetworkManager::setEnableSceneManagement(bool aEnableSceneManagement) {
    mEnableSceneManagement = aEnableSceneManagement;
}

bool NetworkManager::getEnableSceneManagement() const { return mEnableSceneManagement; }

void NetworkManager::setDefaultPlayerPrefab(GameObject& aDefaultPlayerPrefab) {
    mDefaultPlayerPrefab = &aDefaultPlayerPrefab;
}

GameObject& NetworkManager::getDefaultPlayerPrefab() const { return *mDefaultPlayerPrefab; }

void NetworkManager::setRole(NetworkRole aRole) { mRole = aRole; }

NetworkRole NetworkManager::getRole() const { return mRole; }

void NetworkManager::startServer() {
    if (mClient || mHost) {
        throw std::runtime_error("Cannot start server when client or host is already running");
    }
    if (mServer) {
        throw std::runtime_error("Server is already running");
    }
    mServer = std::make_unique<NetworkServer>();
}

void NetworkManager::startClient() {
    if (mServer || mHost) {
        throw std::runtime_error("Cannot start client when server or host is already running");
    }
    if (mClient) {
        throw std::runtime_error("Client is already running");
    }
    mClient = std::make_unique<NetworkClient>();
}

void NetworkManager::startHost() {
    if (mServer || mClient) {
        throw std::runtime_error("Cannot start host when server or client is already running");
    }
    if (mHost) {
        throw std::runtime_error("Host is already running");
    }
    mHost = std::make_unique<NetworkHost>();
}
