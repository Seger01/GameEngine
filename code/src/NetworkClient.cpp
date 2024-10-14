#include "NetworkClient.h"

void NetworkClient::Connect(NetworkManager &manager, const char *serverIP, unsigned short port)
{
    manager.StartClient(serverIP, port);
}

void NetworkClient::SendInput(SLNet::RakPeerInterface *peer, float inputX, float inputY)
{
    SLNet::BitStream bs;
    bs.Write((SLNet::MessageID)ID_USER_PACKET_ENUM + 1);
    bs.Write(inputX);
    bs.Write(inputY);
    peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}