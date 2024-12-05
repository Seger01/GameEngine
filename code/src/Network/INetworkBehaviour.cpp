#include "Network/INetworkBehaviour.h"

int INetworkBehaviour::networkBehaviourIDCounter = 0;

INetworkBehaviour::INetworkBehaviour(std::string aTag) : IBehaviourScript(aTag), mIsOwner(false), mIsOwnerSet(false) {
    mNetworkBehaviourID = networkBehaviourIDCounter++;
}
void INetworkBehaviour::serverRpc() { throw std::runtime_error("INetworkBehaviour::serverRpc() not implemented"); }

void INetworkBehaviour::clientRpc() { throw std::runtime_error("INetworkBehaviour::clientRpc() not implemented"); }

void INetworkBehaviour::onNetworkSpawn()
{
	throw std::runtime_error("INetworkBehaviour::OnNetworkSpawn() not implemented");
}

void INetworkBehaviour::RegisterNetworkVariable(NetworkVariableBase* variable) {
	mNetworkVariables.push_back(*variable);
	variable->setNetworkVariableID(mNetworkVariables.size() - 1);
}

std::vector<std::reference_wrapper<NetworkVariableBase>> INetworkBehaviour::GetNetworkVariables()
{
	return mNetworkVariables;
}

bool INetworkBehaviour::isOwner() {
    return mGameObject->getComponents<NetworkObject>()[0]->isOwner();
    if (mIsOwnerSet) {
        return mIsOwner;
    } else {
        NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
        if (!networkObject) {
            throw std::runtime_error("INetworkBehaviour::isOwner() NetworkObject not found");
        }

        mIsOwner = networkObject->isOwner();
        mIsOwnerSet = true;
        return mIsOwner;
    }
}

void INetworkBehaviour::destroy() { mGameObject->removeComponent(this); }

uint8_t INetworkBehaviour::getNetworkBehaviourID() const { return mNetworkBehaviourID; }
