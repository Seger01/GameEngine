#ifndef FPSCOUNTERBEHAVIOURSCRIPT_H
#define FPSCOUNTERBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"

class FPSCounterBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<FPSCounterBehaviourScript>(*this); }

private:
};

#endif
