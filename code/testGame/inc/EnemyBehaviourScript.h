#pragma once

#include "EnemyTransformSerialize.h"
#include "GameObject.h"
#include "Network/INetworkBehaviour.h"
#include "Network/NetworkVariable.h"
#include <iostream>

class EnemyBehaviourScript : public INetworkBehaviour
{
public:
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<EnemyBehaviourScript>(*this); }
private: 
    void toggleAnimaionEnabled();
    void setAnimationActive(std::string aAnimationTag, bool aState);
    void deactivateAllAnimations();
    void setFlipX(bool aState);
    void setFlipY(bool aState);

private:
	NetworkVariable<EnemyTransformSerialize> mTransform{this};
};