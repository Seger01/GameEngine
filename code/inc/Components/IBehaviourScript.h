#ifndef IBEHAVIOURSCRIPT_H
#define IBEHAVIOURSCRIPT_H

#include "Component.h"
#include "GameObject.h"

class IBehaviourScript : public Component {
public:
    virtual ~IBehaviourScript() {}
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual void onCollide(GameObject* aGameObject) = 0;
};

#endif
