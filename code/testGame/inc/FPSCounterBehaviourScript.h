#ifndef FPSCOUNTERBEHAVIOURSCRIPT_H
#define FPSCOUNTERBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"

class FPSCounterBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;

private:
};

#endif
