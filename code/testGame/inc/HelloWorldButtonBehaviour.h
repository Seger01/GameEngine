#include "Components/IButtonBehaviourScript.h"

#include <iostream>

#include "Components/Sprite.h"
#include "GameObject.h"

class HelloWorldButtonBehaviour : public IButtonBehaviourScript {
public:
    HelloWorldButtonBehaviour() {}
    ~HelloWorldButtonBehaviour() {}

    std::unique_ptr<Component> clone() const override { return std::make_unique<HelloWorldButtonBehaviour>(*this); }

    void onButtonPressed() override { std::cout << "Hello World!" << std::endl; }

    void onButtonReleased() override { std::cout << "Goodbye World!" << std::endl; }

    void onButtonHover() override {
        std::cout << "Hovering over button!" << std::endl;

        for (auto& sprite : mGameObject->getComponents<Sprite>()) {
            sprite->setColorFilter({255, 255, 255, 255});
        }
    }

    void onButtonUnhover() override {
        std::cout << "No longer hovering over button!" << std::endl;

        for (auto& sprite : mGameObject->getComponents<Sprite>()) {
            sprite->setColorFilter({255, 255, 255, 100});
        }
    }
};
