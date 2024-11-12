#include "FPSCounterBehaviourScript.h"

#include <iostream>

#include "GameObject.h"
#include "Text.h"
#include "Time.h"

void FPSCounterBehaviourScript::onStart() {}

void FPSCounterBehaviourScript::onUpdate() {
    if (dynamic_cast<Text*>(mGameObject)) {
        Text* text = dynamic_cast<Text*>(mGameObject);

        // text->setText("FPS: " + std::to_string(static_cast<int>(1 / Time::deltaTime)));
        text->setText("TEXXXXXT");
    }
}
