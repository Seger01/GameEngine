#pragma once

#include "IBehaviourScript.h"
#include "GameObject.h"
#include <iostream>

class BulletBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;
    std::unique_ptr<Component> clone() const override { return std::make_unique<BulletBehaviourScript>(*this); }
};
