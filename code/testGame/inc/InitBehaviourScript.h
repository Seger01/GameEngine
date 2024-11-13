#ifndef INITBEHAVIOURSCRIPT_H
#define INITBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"

class InitBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<InitBehaviourScript>(*this); }

private:
    void createLevel1();
};

#endif
