#ifndef IBUTTONBEHAVIOURSCRIPT_H
#define IBUTTONBEHAVIOURSCRIPT_H

#include "Component.h"

class IButtonBehaviourScript : public Component {
public:
    IButtonBehaviourScript(std::string aTag = "defaultButtonBehaviourScript") : Component(aTag) {}
    virtual ~IButtonBehaviourScript() {}
    virtual std::unique_ptr<Component> clone() const override = 0;
    virtual void onButtonPressed() {}
    virtual void onButtonReleased() {}

    virtual void onButtonHover() {}
    virtual void onButtonUnhover() {}
};

#endif // IBUTTONBEHAVIOURSCRIPT_H
