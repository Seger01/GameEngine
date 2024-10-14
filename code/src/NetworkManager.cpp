#include "NetworkManager.h"

NetworkManager::NetworkManager() : mPeer(SLNet::RakPeerInterface::GetInstance()), mRole(CLIENT) {}

NetworkManager::~NetworkManager()
{
    Shutdown();
}

void NetworkManager::StartServer(unsigned short port)
{
    SLNet::SocketDescriptor sd(port, 0);
    mPeer->Startup(10, &sd, 1);
    mPeer->SetMaximumIncomingConnections(10);
    mRole = SERVER;
    std::cout << "Server started on port " << port << std::endl;
}

void NetworkManager::StartClient(const char *serverIP, unsigned short serverPort)
{
    SLNet::SocketDescriptor sd;
    mPeer->Startup(1, &sd, 1);
    mPeer->Connect(serverIP, serverPort, nullptr, 0);
    mRole = CLIENT;
    std::cout << "Client started and connecting to server: " << serverIP << std::endl;
}

void NetworkManager::StartHost(unsigned short port)
{
    StartServer(port);
    StartClient("127.0.0.1", port);
    mRole = HOST;
    std::cout << "Host started on port " << port << std::endl;
}

void NetworkManager::Shutdown()
{
    mPeer->Shutdown(300);
    SLNet::RakPeerInterface::DestroyInstance(mPeer);
    std::cout << "Network shutdown complete." << std::endl;
}

void NetworkManager::Update()
{
    SLNet::Packet *packet;
    for (packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_NEW_INCOMING_CONNECTION:
            std::cout << "A connection is incoming.\n";
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Our connection request has been accepted.\n";
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            std::cout << "The server is full.\n";
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            std::cout << "A client has disconnected.\n";
            break;
        case ID_CONNECTION_LOST:
            std::cout << "A client lost the connection.\n";
            break;
        default:
            std::cout << "Received packet id: " << (int)packet->data[0] << std::endl;
            break;
        }
    }
}