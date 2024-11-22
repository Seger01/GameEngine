#include "Network/NetworkObject.h"

NetworkObject::NetworkObject() : mIsOwner(false), mClientID(SLNet::UNASSIGNED_RAKNET_GUID), mIsPlayer(false) {
    this->setTag("DefaultNetworkObject");
}

std::unique_ptr<Component> NetworkObject::clone() const { return std::make_unique<NetworkObject>(*this); }

void NetworkObject::setClientID(SLNet::RakNetGUID aClientID) { mClientID = aClientID; }

SLNet::RakNetGUID NetworkObject::getClientID() const { return mClientID; }

void NetworkObject::setOwner(bool aIsOwner) { mIsOwner = aIsOwner; }

bool NetworkObject::isOwner() const { return mIsOwner; }

bool NetworkObject::isPlayer() const { return mIsPlayer; }

void NetworkObject::addNetworkBehaviour(INetworkBehaviour* aNetworkBehaviour) {
    mNetworkBehaviours.push_back(aNetworkBehaviour);
    aNetworkBehaviour->setOwner(mIsOwner);
}

void NetworkObject::setPlayer(bool aIsPlayer) { mIsPlayer = aIsPlayer; }