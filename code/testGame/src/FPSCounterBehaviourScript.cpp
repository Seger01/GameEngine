#include "FPSCounterBehaviourScript.h"

#include <iostream>

#include "EngineBravo.h"
#include "GameObject.h"
#include "Text.h"
#include "Time.h"

void FPSCounterBehaviourScript::onStart() {}

void FPSCounterBehaviourScript::onUpdate() {
    if (dynamic_cast<Text*>(mGameObject)) {
        Text* text = dynamic_cast<Text*>(mGameObject);

        text->setText("FPS: " + std::to_string(static_cast<int>(1 / Time::deltaTime)));

        EngineBravo& engine = EngineBravo::getInstance();

        int textWidth = 0;
        int textHeight = 0;
        if (!engine.getRenderSystem().getTextSize(text->getFont(), text->getText(), textWidth, textHeight,
                                                  text->getScale())) {
            std::cout << "Failed to get text size for FPS counter.\n";
        }

        Sprite* sprite = text->getComponents<Sprite>()[0];

        sprite->setWidth(textWidth + 50);
        sprite->setHeight(textHeight + 10);

        Transform spritePos = sprite->getRelativePosition();
        spritePos.position.x = -25;
        spritePos.position.y = -5;

        sprite->setRelativePosition(spritePos);
    }
}

void FPSCounterBehaviourScript::onCollide(GameObject* aGameObject) {
    std::cout << "FPSCounterBehaviourScript collided with " << aGameObject->getName() << std::endl;
}
