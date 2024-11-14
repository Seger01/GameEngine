#ifndef STARTUPBEHAVIOURSCRIPT_H
#define STARTUPBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"
#include <chrono>

class StartupBehaviourScript : public IBehaviourScript {
public:
    StartupBehaviourScript() {}
    ~StartupBehaviourScript() {}

    std::unique_ptr<Component> clone() const override { return std::make_unique<StartupBehaviourScript>(*this); }

    void onStart() override;

    void onUpdate() override;

private:
    std::chrono::time_point<std::chrono::system_clock> mStartTime;
};

#endif // STARTUPBEHAVIOURSCRIPT_H