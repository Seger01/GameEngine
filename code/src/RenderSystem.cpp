#include "RenderSystem.h"

#include <iostream>
#include <memory>

#include "Animation.h"
#include "Color.h"
#include "SDL_timer.h"
#include "Sprite.h"

RenderSystem::RenderSystem() {
    mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
    mRenderer = std::make_unique<Renderer>(*mWindow);

    mBackgroundColor = Color(0, 0, 0);

    return;
}

void RenderSystem::renderSprite(GameObject* aGameObject, Sprite* aSprite) {
    mRenderer->renderTexture(*aSprite->getTexture(), aSprite->getSource(),
                             aGameObject->getTransform().position + aSprite->getRelativePosition().position,
                             aSprite->getWidth(), aSprite->getHeight(), aSprite->getFlipX(), aSprite->getFlipY(),
                             aGameObject->getTransform().rotation + aSprite->getRelativePosition().rotation);
}

void RenderSystem::renderAnimation(GameObject* aGameObject, Animation* aAnimation) {
    Sprite* currentFrame = aAnimation->getFrameAtTime(SDL_GetTicks());

    renderSprite(aGameObject, currentFrame);
}

void RenderSystem::render(Scene* aScene) {
    // mRenderer->clear(Color(0, 0, 0));
    mRenderer->clear(mBackgroundColor);
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Animation>()) {
            Animation* animation = gameObject->getComponent<Animation>();

            std::cout << "GameObject pos: " << gameObject->getTransform().position.x << ", "
                      << gameObject->getTransform().position.y << std::endl;

            renderAnimation(gameObject, animation);
        } else if (gameObject->hasComponent<Sprite>()) {
            Sprite* sprite = gameObject->getComponent<Sprite>();

            renderSprite(gameObject, sprite);
        }
    }
    mRenderer->show();
}

Renderer& RenderSystem::getRenderer() { return *mRenderer; }
Window& RenderSystem::getWindow() { return *mWindow; }
