#ifndef TESTBEHAVIOURSCRIPT_H
#define TESTBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"

class TestBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<TestBehaviourScript>(*this); }
};

#endif // !TESTBEHAVIOURSCRIPT_H
