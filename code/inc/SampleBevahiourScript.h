#pragma once
#include <iostream>

#include "IBehaviourScript.h"

class SampleBehaviourScript : public IBehaviourScript {
public:
    void onStart() override {
        // Initialization code here
        std::cout << "SampleBehaviourScript started!" << std::endl;
    }

    void onUpdate() override {
        // Update code here
        std::cout << "SampleBehaviourScript updating!" << std::endl;
    }
};
