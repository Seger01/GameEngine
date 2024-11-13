#ifndef INITBEHAVIOURSCRIPT_H
#define INITBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"
#include "GameObject.h"

class InitBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

private:
    void createLevel1();
};

#endif
