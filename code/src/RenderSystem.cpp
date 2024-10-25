#include "RenderSystem.h"

#include <iostream>
#include <memory>

#include "Color.h"
#include "Sprite.h"

RenderSystem::RenderSystem() {
    mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
    mRenderer = std::make_unique<Renderer>(*mWindow);

    mBackgroundColor = Color(0, 0, 0);

    return;
}

void RenderSystem::render(Scene* aScene) {
    mRenderer->clear(Color(0, 0, 0));
    mRenderer->clear(mBackgroundColor);
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Sprite>()) {
            std::cout << "Game object in scene with sprite" << std::endl;
            Sprite* sprite = gameObject->getComponent<Sprite>();

            mRenderer->renderTexture(*sprite->getTexture(), sprite->getTransform().position, sprite->getWidth(),
                                     sprite->getHeight(), sprite->getFlipX(), sprite->getFlipY(),
                                     sprite->getTransform().rotation);
            // mRenderer->renderSquare(Vector2(200, 200), 50, 50, Color(255, 255, 255, 0), false);
            // mRenderer->renderSquare(sprite->getTransform().position, 50, 50, Color(255, 255, 255, 0), true);
        }
    }
    mRenderer->show();
}

Renderer& RenderSystem::getRenderer() { return *mRenderer; }
Window& RenderSystem::getWindow() { return *mWindow; }
