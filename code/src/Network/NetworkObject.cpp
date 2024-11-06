#include "NetworkObject.h"
#include <stdexcept>

void NetworkObject::spawn()
{
    throw std::runtime_error("NetworkObject::spawn() not implemented");
}

void NetworkObject::despawn()
{
    throw std::runtime_error("NetworkObject::despawn() not implemented");
}

int NetworkObject::getNetworkID()
{
    throw std::runtime_error("NetworkObject::getNetworkID() not implemented");
}