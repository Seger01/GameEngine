#ifndef IBEHAVIOURSCRIPT_H
#define IBEHAVIOURSCRIPT_H

#include "Component.h"

class IBehaviourScript : public Component {
public:
    virtual ~IBehaviourScript() {}
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;

    bool hasScriptStarted() { return hasStarted; }
    void setScriptStarted(bool aState) { hasStarted = aState; }

    // Virtual factory method
    virtual std::unique_ptr<Component> clone() const override {
        return std::make_unique<IBehaviourScript>(*this); // Uses the copy constructor of IBehaviourScript
    }

private:
    bool hasStarted = false;
};

#endif
