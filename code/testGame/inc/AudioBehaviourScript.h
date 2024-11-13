#pragma once

#include "IBehaviourScript.h"

class AudioBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;

    std::unique_ptr<Component> clone() const override;
};