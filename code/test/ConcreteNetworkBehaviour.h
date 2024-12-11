#ifndef CONCRETENETWORKBEHAVIOUR_H
#define CONCRETENETWORKBEHAVIOUR_H

#include "ConcreteNetworkSerializable.h"
#include "Network/INetworkBehaviour.h"
#include "Network/NetworkVariable.h"

class ConcreteNetworkBehaviour : public INetworkBehaviour {
public:
	void onStart() override;

    void onUpdate() override;

    void onCollide(GameObject* aGameObject) override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<ConcreteNetworkBehaviour>(); }

private:
    NetworkVariable<ConcreteNetworkSerializable> mVariable{this};
};

#endif // CONCRETENETWORKBEHAVIOUR_H