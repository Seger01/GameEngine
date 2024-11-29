#include "ConcreteNetworkBehaviour.h"

void ConcreteNetworkBehaviour::OnNetworkSpawn() {
    // Do something
}

void ConcreteNetworkBehaviour::onStart() {
    ConcreteNetworkSerializable serializable;
    serializable.setValue(20);
    mVariable.setValue(ConcreteNetworkSerializable());
}

void ConcreteNetworkBehaviour::onUpdate() {
    // Do something
}

void ConcreteNetworkBehaviour::onCollide(GameObject* aGameObject) {
    // Do something
}