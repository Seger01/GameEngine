#include "Engine/UIManager.h"

#include "Button.h"
#include "Components/IButtonBehaviourScript.h"
#include "Engine/EngineBravo.h"
#include "EventManager.h"
#include "Scene.h"

UIManager::UIManager() {}

UIManager::~UIManager() {}

void UIManager::init() {
    EventManager& eventManager = EngineBravo::getInstance().getEventManager();

    eventManager.subscribe(std::bind(&UIManager::handleEvent, this, std::placeholders::_1), EventType::MouseButtonDown);
}

void UIManager::handleEvent(const Event& aEvent) { mEventQueue.push_back(aEvent); }

void UIManager::update(Scene* aScene) {
    for (GameObject* gameObject : aScene->getGameObjects()) {
        if (dynamic_cast<Button*>(gameObject)) {
            Button* button = dynamic_cast<Button*>(gameObject);

            Camera& currentCamera = aScene->getActiveCamera();

            Vector2 cameraOrigin = currentCamera.getOrigin();

            int windowWidth = EngineBravo::getInstance().getRenderSystem().getWindow().getSize().x;
            int windowHeight = EngineBravo::getInstance().getRenderSystem().getWindow().getSize().y;

            int buttonWidth = button->getWidth();
            int buttonHeight = button->getHeight();

            Vector2 buttonPosition = button->getTransform().position;
            Vector2 drawPosition = buttonPosition - cameraOrigin;

            drawPosition.x = std::round(drawPosition.x * (static_cast<float>(windowWidth) / currentCamera.getWidth()));
            drawPosition.y =
                std::round(drawPosition.y * (static_cast<float>(windowHeight) / currentCamera.getHeight()));

            buttonWidth = std::round(buttonWidth * (static_cast<float>(windowWidth) / currentCamera.getWidth()));

            buttonHeight = std::round(buttonHeight * (static_cast<float>(windowHeight) / currentCamera.getHeight()));

            for (Event event : mEventQueue) {
                Point mouseScreenPos = event.mouse.position;

                if (button->interactable()) {
                    if (mouseScreenPos.x >= drawPosition.x && mouseScreenPos.x <= drawPosition.x + buttonWidth &&
                        mouseScreenPos.y >= drawPosition.y && mouseScreenPos.y <= drawPosition.y + buttonHeight) {
                        if (button->getComponents<IButtonBehaviourScript>().size() > 0) {
                            if (event.type == EventType::MouseButtonDown) {
                                for (IButtonBehaviourScript* buttonBehaviourScript :
                                     button->getComponents<IButtonBehaviourScript>()) {
                                    buttonBehaviourScript->onButtonPressed();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    mEventQueue.clear();
}
