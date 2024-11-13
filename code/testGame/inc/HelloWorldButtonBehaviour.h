#include "Components/IButtonBehaviourScript.h"

#include <iostream>

class HelloWorldButtonBehaviour : public IButtonBehaviourScript {
public:
    HelloWorldButtonBehaviour() {}
    ~HelloWorldButtonBehaviour() {}

    std::unique_ptr<Component> clone() const override { return std::make_unique<HelloWorldButtonBehaviour>(*this); }

    void onButtonPressed() override { std::cout << "Hello World!" << std::endl; }

    void onButtonReleased() override { std::cout << "Goodbye World!" << std::endl; }
};
