#include "RenderSystem.h"

#include <iostream>
#include <memory>

#include "Animation.h"
#include "BoxCollider.h"
#include "Color.h"
#include "ParticleEmitter.h"
#include "SDL_timer.h"
#include "Sprite.h"
#include "Time.h"

RenderSystem::RenderSystem() : WindowWidth(640), WindowHeight(360) {
    mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
    mRenderer = std::make_unique<Renderer>(*mWindow);

    mBackgroundColor = Color(255, 255, 255);

    return;
}

void RenderSystem::renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite) {
    int spriteWidth = aSprite->getWidth();
    int spriteHeight = aSprite->getHeight();

    int WindowWidth = mWindow->getSize().x;
    int WindowHeight = mWindow->getSize().y;

    Vector2 texturePosition = aGameObject->getTransform().position + aSprite->getRelativePosition().position;

    Vector2 cameraOrigin = aCurrentCamera.getTransform().position -
                           Vector2(aCurrentCamera.getWidth() / 2.0f, aCurrentCamera.getHeight() / 2.0f);

    Vector2 drawPosition = texturePosition - cameraOrigin;

    drawPosition.x = drawPosition.x * (static_cast<float>(WindowWidth) / aCurrentCamera.getWidth());
    drawPosition.y = drawPosition.y * (static_cast<float>(WindowHeight) / aCurrentCamera.getHeight());

    spriteWidth =
        std::ceil(static_cast<int>(static_cast<float>(spriteWidth) *
                                   (static_cast<float>(WindowWidth) / static_cast<float>(aCurrentCamera.getWidth()))));
    spriteHeight = std::ceil(
        static_cast<int>(static_cast<float>(spriteHeight) *
                         (static_cast<float>(WindowHeight) / static_cast<float>(aCurrentCamera.getHeight()))));

    // std::cout << "RenderSystem::renderSprite: drawPosition: " << drawPosition.x << ", " << drawPosition.y <<
    // std::endl; std::cout << "RenderSystem::renderSprite: spriteWidth: " << spriteWidth << ", spriteHeight: " <<
    // spriteHeight
    //           << std::endl;

    mRenderer->renderTexture(*aSprite->getTexture(), aSprite->getSource(), drawPosition, spriteWidth, spriteHeight,
                             aSprite->getFlipX(), aSprite->getFlipY(),
                             aGameObject->getTransform().rotation + aSprite->getRelativePosition().rotation);

    // mRenderer->renderSquare(drawPosition, spriteWidth, spriteHeight, Color(255, 0, 0), false);
    // drawPosition.x += 30;
    // mRenderer->renderText("" + std::to_string(aSprite->getTexture()->getID()) + ", " + std::to_string(0),
    // drawPosition,
    //                       Color(0, 255, 0));
}

void RenderSystem::renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation) {
    Sprite* currentFrame = aAnimation->getFrameAtTime(SDL_GetTicks());
    // Sprite* currentFrame = aAnimation->getFrame(0);

    renderSprite(aCurrentCamera, aGameObject, currentFrame);
}

void RenderSystem::renderParticle(Camera& aCurrentCamera, Particle& aParticle) {
    float particleWidth = aParticle.getSize().x;
    float particleHeight = aParticle.getSize().y;

    int WindowWidth = mWindow->getSize().x;
    int WindowHeight = mWindow->getSize().y;

    Vector2 particlePosition = aParticle.getPosition();

    Vector2 cameraOrigin = aCurrentCamera.getTransform().position -
                           Vector2(aCurrentCamera.getWidth() / 2.0f, aCurrentCamera.getHeight() / 2.0f);

    Vector2 drawPosition = particlePosition - cameraOrigin;

    drawPosition.x = drawPosition.x * (static_cast<float>(WindowWidth) / aCurrentCamera.getWidth());
    drawPosition.y = drawPosition.y * (static_cast<float>(WindowHeight) / aCurrentCamera.getHeight());

    particleWidth = static_cast<int>(static_cast<float>(particleWidth) *
                                     (static_cast<float>(WindowWidth) / static_cast<float>(aCurrentCamera.getWidth())));
    particleHeight =
        static_cast<int>(static_cast<float>(particleHeight) *
                         (static_cast<float>(WindowHeight) / static_cast<float>(aCurrentCamera.getHeight())));

    if (aParticle.getRotation() == 0) {
        mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
                                aParticle.getColor(), true);
    } else {
        mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
                                aParticle.getRotation(), aParticle.getColor(), true);
    }
}

int RenderSystem::getLowestLayer(Scene* aScene) {
    int lowestLayer = 0;
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Sprite>()) {
            for (auto sprite : gameObject->getComponents<Sprite>()) {
                if (sprite->getLayer() < lowestLayer) {
                    lowestLayer = sprite->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<Animation>()) {
            for (auto animation : gameObject->getComponents<Animation>()) {
                if (animation->getLayer() < lowestLayer) {
                    lowestLayer = animation->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<ParticleEmitter>()) {
            for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>()) {
                if (particleEmitter->getLayer() < lowestLayer) {
                    lowestLayer = particleEmitter->getLayer();
                }
            }
        }
    }
    return lowestLayer;
}

int RenderSystem::getHighestLayer(Scene* aScene) {
    int highestLayer = 0;
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Sprite>()) {
            for (auto sprite : gameObject->getComponents<Sprite>()) {
                if (sprite->getLayer() > highestLayer) {
                    highestLayer = sprite->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<Animation>()) {
            for (auto animation : gameObject->getComponents<Animation>()) {
                if (animation->getLayer() > highestLayer) {
                    highestLayer = animation->getLayer();
                }
            }
        }
        if (gameObject->hasComponent<ParticleEmitter>()) {
            for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>()) {
                if (particleEmitter->getLayer() > highestLayer) {
                    highestLayer = particleEmitter->getLayer();
                }
            }
        }
    }
    return highestLayer;
}

void RenderSystem::renderLayer(Scene* aScene, int aLayer) {
    Camera& activeCamera = aScene->getActiveCamera();

    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->hasComponent<Animation>()) {
            for (auto animation : gameObject->getComponents<Animation>()) {
                if (animation->isActive() && animation->getLayer() == aLayer) {
                    renderAnimation(activeCamera, gameObject, animation);
                }
            }
        } else if (gameObject->hasComponent<Sprite>()) {
            for (auto sprite : gameObject->getComponents<Sprite>()) {
                if (sprite->isActive() && sprite->getLayer() == aLayer) {
                    renderSprite(activeCamera, gameObject, sprite);
                }
            }
        }
        if (gameObject->hasComponent<ParticleEmitter>()) {
            for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>()) {
                if (particleEmitter->isActive() && particleEmitter->getLayer() == aLayer) {
                    for (auto& particle : particleEmitter->getParticles()) {
                        renderParticle(activeCamera, particle);
                    }
                }
            }
        }
    }
}

void RenderSystem::render(Scene* aScene) {
    mRenderer->clear(mBackgroundColor);

    int lowestLayer = getLowestLayer(aScene);
    int highestLayer = getHighestLayer(aScene);

    for (int layer = lowestLayer; layer <= highestLayer; ++layer) {
        renderLayer(aScene, layer);
    }

    renderDeubgInfo(aScene);

    mRenderer->show();
}

void RenderSystem::renderDeubgInfo(Scene* aScene) {
    if (Time::deltaTime == 0) {
        return;
    }

    // std::cout << "RenderSystem::renderDeubgInfo: Time::deltaTime: " << Time::deltaTime << std::endl;
    // int fps = 1.0f / Time::deltaTime;
    //
    // // Render FPS counter in the top left corner of the screen with black text color (0, 0, 0)
    // mRenderer->renderText("FPS: " + std::to_string(fps), Vector2(10, 10), Color(0, 255, 0));
    if (true) {
        for (auto& gameObject : aScene->getGameObjects()) {
            // std::cout << "Amount of BoxColliders: " << gameObject->getComponents<BoxCollider>().size() << std::endl;
            if (gameObject->hasComponent<BoxCollider>()) {
                for (auto boxCollider : gameObject->getComponents<BoxCollider>()) {
                    Camera& aCurrentCamera = aScene->getActiveCamera();

                    int spriteWidth = boxCollider->getWidth();
                    int spriteHeight = boxCollider->getHeight();

                    int WindowWidth = mWindow->getSize().x;
                    int WindowHeight = mWindow->getSize().y;

                    Vector2 texturePosition =
                        gameObject->getTransform().position + boxCollider->getTransform().position;

                    Vector2 cameraOrigin = aCurrentCamera.getTransform().position -
                                           Vector2(aCurrentCamera.getWidth() / 2.0f, aCurrentCamera.getHeight() / 2.0f);

                    Vector2 drawPosition = texturePosition - cameraOrigin;

                    drawPosition.x = drawPosition.x * (static_cast<float>(WindowWidth) / aCurrentCamera.getWidth());
                    drawPosition.y = drawPosition.y * (static_cast<float>(WindowHeight) / aCurrentCamera.getHeight());

                    spriteWidth = std::ceil(static_cast<int>(
                        static_cast<float>(spriteWidth) *
                        (static_cast<float>(WindowWidth) / static_cast<float>(aCurrentCamera.getWidth()))));
                    spriteHeight = std::ceil(static_cast<int>(
                        static_cast<float>(spriteHeight) *
                        (static_cast<float>(WindowHeight) / static_cast<float>(aCurrentCamera.getHeight()))));

                    mRenderer->renderSquare(drawPosition, spriteWidth, spriteHeight, Color(0, 0, 255), false);
                }
            }
        }
    }
}

Renderer& RenderSystem::getRenderer() { return *mRenderer; }
Window& RenderSystem::getWindow() { return *mWindow; }
