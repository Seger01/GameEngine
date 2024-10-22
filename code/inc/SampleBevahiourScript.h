#pragma once
#include "IBehaviourScript.h"
#include <iostream>

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