#ifndef CANVASBEHAVIOURSCRIPT_H
#define CANVASBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"

class CanvasBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<CanvasBehaviourScript>(*this); }
};

#endif // CANVASBEHAVIOURSCRIPT_H
