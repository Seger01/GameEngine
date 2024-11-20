#ifndef IBEHAVIOURSCRIPT_H
#define IBEHAVIOURSCRIPT_H

#include "Component.h"
#include "GameObject.h"

class IBehaviourScript : public Component {
public:
    virtual ~IBehaviourScript() = default;
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual void onCollide(GameObject* aGameObject) {}

    virtual std::unique_ptr<Component> clone() const override = 0;

    bool hasScriptStarted() { return hasStarted; }
    void setScriptStarted(bool aState) { hasStarted = aState; }

private:
    bool hasStarted = false;
};

#endif
