#include "Components/IButtonBehaviourScript.h"

#include <iostream>

class HelloWorldButtonBehaviour : public IButtonBehaviourScript {
public:
    HelloWorldButtonBehaviour() {}
    ~HelloWorldButtonBehaviour() {}

    void onButtonPressed() override { std::cout << "Hello World!" << std::endl; }

    void onButtonReleased() override { std::cout << "Goodbye World!" << std::endl; }
};
