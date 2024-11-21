#include "Engine/UIManager.h"

#include "Button.h"
#include "Components/IButtonBehaviourScript.h"
#include "Engine/EngineBravo.h"
#include "EventManager.h"
#include "Input.h"
#include "Scene.h"

UIManager::UIManager() {}

UIManager::~UIManager() {}

void UIManager::init() {
    EventManager& eventManager = EngineBravo::getInstance().getEventManager();

    eventManager.subscribe(std::bind(&UIManager::handleMouseDownEvent, this, std::placeholders::_1),
                           EventType::MouseButtonDown);
    eventManager.subscribe(std::bind(&UIManager::handleMouseUpEvent, this, std::placeholders::_1),
                           EventType::MouseButtonUp);
}

void UIManager::handleMouseDownEvent(const Event& aEvent) { mMouseDownEventQueue.push_back(aEvent); }
void UIManager::handleMouseUpEvent(const Event& aEvent) { mMouseUpEventQueue.push_back(aEvent); }

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

            for (Event event : mMouseDownEventQueue) {
                Point mouseScreenPos = event.mouse.position;

                if (button->interactable()) {
                    if (mouseScreenPos.x >= drawPosition.x && mouseScreenPos.x <= drawPosition.x + buttonWidth &&
                        mouseScreenPos.y >= drawPosition.y && mouseScreenPos.y <= drawPosition.y + buttonHeight) {
                        if (button->getComponents<IButtonBehaviourScript>().size() > 0) {
                            if (event.type == EventType::MouseButtonDown) {
                                for (IButtonBehaviourScript* buttonBehaviourScript :
                                     button->getComponents<IButtonBehaviourScript>()) {
                                    buttonBehaviourScript->onButtonPressed();
                                    button->activateOnClickCallback();
                                }
                            }
                        }
                    }
                }
            }

            for (Event event : mMouseUpEventQueue) {
                Point mouseScreenPos = event.mouse.position;

                if (button->interactable()) {
                    if (mouseScreenPos.x >= drawPosition.x && mouseScreenPos.x <= drawPosition.x + buttonWidth &&
                        mouseScreenPos.y >= drawPosition.y && mouseScreenPos.y <= drawPosition.y + buttonHeight) {
                        if (button->getComponents<IButtonBehaviourScript>().size() > 0) {
                            if (event.type == EventType::MouseButtonUp) {
                                for (IButtonBehaviourScript* buttonBehaviourScript :
                                     button->getComponents<IButtonBehaviourScript>()) {
                                    buttonBehaviourScript->onButtonReleased();
                                    button->activateOnReleaseCallback();
                                }
                            }
                        }
                    }
                }
            }

            Point currentMousePos = Input::getInstance().MousePosition();

            if (button->interactable()) {
                if (currentMousePos.x >= drawPosition.x && currentMousePos.x <= drawPosition.x + buttonWidth &&
                    currentMousePos.y >= drawPosition.y && currentMousePos.y <= drawPosition.y + buttonHeight) {
                    if (button->getComponents<IButtonBehaviourScript>().size() > 0) {
                        for (IButtonBehaviourScript* buttonBehaviourScript :
                             button->getComponents<IButtonBehaviourScript>()) {
                            if (!button->isHovered()) {
                                buttonBehaviourScript->onButtonHover();
                                button->setHovered(true);
                            }
                        }
                    }
                } else {
                    if (button->getComponents<IButtonBehaviourScript>().size() > 0) {
                        for (IButtonBehaviourScript* buttonBehaviourScript :
                             button->getComponents<IButtonBehaviourScript>()) {
                            if (button->isHovered()) {
                                buttonBehaviourScript->onButtonUnhover();
                                button->setHovered(false);
                            }
                        }
                    }
                }
            }
        }
    }
    mMouseDownEventQueue.clear();
    mMouseUpEventQueue.clear();
}
