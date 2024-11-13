#pragma once

#include "Components/IBehaviourScript.h"
#include "GameObject.h"
#include <iostream>

class EnemyBehaviourScript : public IBehaviourScript {
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
};