#include "NetworkObject.h"
#include <stdexcept>

NetworkObject::NetworkObject() : mIsOwner(false) { this->setTag("NetworkObject"); }

std::unique_ptr<Component> NetworkObject::clone() const { return std::make_unique<NetworkObject>(*this); }

void NetworkObject::setClientID(SLNet::RakNetGUID aClientID) { mClientID = aClientID; }

SLNet::RakNetGUID NetworkObject::getClientID() const { return mClientID; }

void NetworkObject::setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }

bool NetworkObject::isOwner() const { return mIsOwner; }