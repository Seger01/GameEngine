#pragma once

#include "IBehaviourScript.h"
#include "GameObject.h"
#include <iostream>
#include "EngineBravo.h"
#include "SceneManager.h"

class BulletBehaviourScript : public IBehaviourScript {
public:
    BulletBehaviourScript() {}
    ~BulletBehaviourScript() {}
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;
    std::unique_ptr<Component> clone() const override { return std::make_unique<BulletBehaviourScript>(*this); }
};
