#ifndef CANVASBEHAVIOURSCRIPT_H
#define CANVASBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"

class CanvasBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;
};

#endif // CANVASBEHAVIOURSCRIPT_H
