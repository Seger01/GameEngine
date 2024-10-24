#include "RenderSystem.h"

#include <iostream>
#include <memory>

#include "Sprite.h"

RenderSystem::RenderSystem() {
    mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
    mRenderer = std::make_unique<Renderer>(*mWindow.get());

    return;
}

void RenderSystem::render(Scene* aScene) {
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Sprite>()) {
            std::cout << "Game object in scene with sprite" << std::endl;
            Sprite* sprite = gameObject->getComponent<Sprite>();
        }
    }
}

Renderer& RenderSystem::getRenderer() { return *mRenderer; }
Window& RenderSystem::getWindow() { return *mWindow; }
