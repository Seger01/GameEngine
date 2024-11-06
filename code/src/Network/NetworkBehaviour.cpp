#include "NetworkBehaviour.h"
#include <stdexcept>

void NetworkBehaviour::serverRpc()
{
    throw std::runtime_error("NetworkBehaviour::serverRpc() not implemented");
}

void NetworkBehaviour::clientRpc()
{
    throw std::runtime_error("NetworkBehaviour::clientRpc() not implemented");
}