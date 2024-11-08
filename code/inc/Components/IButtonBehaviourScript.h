#ifndef IBUTTONBEHAVIOURSCRIPT_H
#define IBUTTONBEHAVIOURSCRIPT_H

#include "Component.h"

class IButtonBehaviourScript : public Component {
public:
    virtual ~IButtonBehaviourScript() {}
    virtual void onButtonPressed() = 0;
    virtual void onButtonReleased() = 0;
};

#endif // IBUTTONBEHAVIOURSCRIPT_H
