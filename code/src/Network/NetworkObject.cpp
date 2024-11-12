#include "NetworkObject.h"
#include <stdexcept>

NetworkObject::NetworkObject() : mIsOwner(false) { this->setTag("NetworkObject"); }

void NetworkObject::setClientID(SLNet::RakNetGUID aClientID) { mClientID = aClientID; }

SLNet::RakNetGUID NetworkObject::getClientID() const { return mClientID; }

void NetworkObject::setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }

bool NetworkObject::isOwner() const { return mIsOwner; }