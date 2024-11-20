#include "INetworkBehaviour.h"
#include <stdexcept>

void INetworkBehaviour::serverRpc() { throw std::runtime_error("INetworkBehaviour::serverRpc() not implemented"); }

void INetworkBehaviour::clientRpc() { throw std::runtime_error("INetworkBehaviour::clientRpc() not implemented"); }