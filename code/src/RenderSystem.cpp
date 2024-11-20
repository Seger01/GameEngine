#include "RenderSystem.h"

#include <iostream>
#include <memory>

#include "Animation.h"
#include "BoxCollider.h"
#include "Button.h"
#include "Color.h"
#include "EngineBravo.h"
#include "ParticleEmitter.h"
#include "SDL_timer.h"
#include "ScopedTimer.h"
#include "Sprite.h"
#include "Text.h"
#include "Time.h"
#include "UIObject.h"

RenderSystem::RenderSystem() : WindowWidth(800), WindowHeight(480) {
    mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
    mRenderer = std::make_unique<Renderer>(*mWindow);

    mBackgroundColor = Color(255, 255, 255);

    return;
}

void RenderSystem::renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite) {
    int spriteWidth = aSprite->getWidth();
    int spriteHeight = aSprite->getHeight();

    int windowWidth = mWindow->getSize().x;
    int windowHeight = mWindow->getSize().y;

    Vector2 texturePosition = aGameObject->getTransform().position + aSprite->getRelativePosition().position;

    // Calculate the camera's origin and position the sprite relative to it
    Vector2 cameraOrigin = aCurrentCamera.getOrigin();

    Vector2 drawPosition = texturePosition - cameraOrigin;

    // Snap position to integer to avoid subpixel offsets
    drawPosition.x = std::round(drawPosition.x * (static_cast<float>(windowWidth) / aCurrentCamera.getWidth()));
    drawPosition.y = std::round(drawPosition.y * (static_cast<float>(windowHeight) / aCurrentCamera.getHeight()));

    // Adjust the width and height slightly to cover gaps
    spriteWidth = std::round(spriteWidth * (static_cast<float>(windowWidth) / aCurrentCamera.getWidth())) + 1;
    spriteHeight = std::round(spriteHeight * (static_cast<float>(windowHeight) / aCurrentCamera.getHeight())) + 1;

    // Render the sprite with adjusted size
    mRenderer->renderTexture(*aSprite->getTexture(), aSprite->getSource(), drawPosition, spriteWidth, spriteHeight,
                             aSprite->getFlipX(), aSprite->getFlipY(),
                             aGameObject->getTransform().rotation + aSprite->getRelativePosition().rotation);
}

void RenderSystem::renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation) {
    Sprite* currentFrame = aAnimation->getCurrentFrame();
    // Sprite* currentFrame = aAnimation->getFrame(0);

    renderSprite(aCurrentCamera, aGameObject, currentFrame);
}

void RenderSystem::renderParticle(Camera& aCurrentCamera, Particle& aParticle) {
    float particleWidth = aParticle.getSize().x;
    float particleHeight = aParticle.getSize().y;

    int WindowWidth = mWindow->getSize().x;
    int WindowHeight = mWindow->getSize().y;

    Vector2 particlePosition = aParticle.getPosition();
    Vector2 cameraOrigin = aCurrentCamera.getOrigin();
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

void RenderSystem::renderText(Camera& aCurrentCamera, const std::string& aText, Vector2 aLocation, Color aColor,
                              Vector2 aScale) {
    float scaleX = aScale.x * (mWindow->getSize().x / static_cast<float>(aCurrentCamera.getWidth()));
    float scaleY = aScale.y * (mWindow->getSize().y / static_cast<float>(aCurrentCamera.getHeight()));

    Vector2 cameraOrigin = aCurrentCamera.getOrigin();
    Vector2 drawPosition = aLocation - cameraOrigin;

    drawPosition.x = drawPosition.x * (static_cast<float>(mWindow->getSize().x) / aCurrentCamera.getWidth());
    drawPosition.y = drawPosition.y * (static_cast<float>(mWindow->getSize().y) / aCurrentCamera.getHeight());

    mRenderer->renderText(aText, drawPosition, aColor, scaleX, scaleY);
}

// void RenderSystem::renderButton(Camera& aCurrentCamera, Button* aButton) {
//     float scaleX = mWindow->getSize().x / static_cast<float>(aCurrentCamera.getWidth());
//     float scaleY = mWindow->getSize().y / static_cast<float>(aCurrentCamera.getHeight());
//
//     Vector2 cameraOrigin = aCurrentCamera.getOrigin();
//     Vector2 drawPosition = aButton->getTransform().position - cameraOrigin;
//
//     drawPosition.x = drawPosition.x * (static_cast<float>(mWindow->getSize().x) / aCurrentCamera.getWidth());
//     drawPosition.y = drawPosition.y * (static_cast<float>(mWindow->getSize().y) / aCurrentCamera.getHeight());
//
//     mRenderer->renderSquare(drawPosition, static_cast<int>(aButton->getWidth() * scaleX),
//                             static_cast<int>(aButton->getHeight() * scaleY), Color(255, 0, 0), false);
// }

bool RenderSystem::getTextSize(const std::string& aFont, const std::string& aText, int& aWidth, int& aHeight,
                               Vector2 aScale) {
    if (!mRenderer->calculateTextSize(aFont, aText, aWidth, aHeight)) {
        return false;
    }

    aWidth = aWidth * aScale.x;
    aHeight = aHeight * aScale.y;

    return true;
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

    for (GameObject& gameObject : mObjects) {
        // if (gameObject.hasComponent<Animation>()) {
        for (auto animation : gameObject.getComponents<Animation>()) {
            if (animation->isActive() && animation->getLayer() == aLayer) {
                renderAnimation(activeCamera, &gameObject, animation);
            }
        }
        //} else if (gameObject.hasComponent<Sprite>()) {
        for (auto sprite : gameObject.getComponents<Sprite>()) {
            if (sprite->isActive() && sprite->getLayer() == aLayer) {
                renderSprite(activeCamera, &gameObject, sprite);
            }
        }
        //}
        // if (gameObject.hasComponent<ParticleEmitter>()) {
        for (auto particleEmitter : gameObject.getComponents<ParticleEmitter>()) {
            if (particleEmitter->isActive() && particleEmitter->getLayer() == aLayer) {
                for (auto& particle : particleEmitter->getParticles()) {
                    renderParticle(activeCamera, particle);
                }
            }
        }
        //}
        if (dynamic_cast<Text*>(&gameObject)) {
            // try {
            Text* text = dynamic_cast<Text*>(&gameObject);
            if (text->isActive() && text->getLayer() == aLayer) {
                renderText(activeCamera, text->getText(), text->getTransform().position, text->getColor(),
                           text->getScale());
            }
            //} catch (std::bad_cast& e) {
            // std::cout << "Failed to cast to Text" << std::endl;
        }
    }
}

void RenderSystem::render(Scene* aScene) {
    // ScopedTimer scopedTimer("RenderSystem::render");
    mRenderer->clear(mBackgroundColor);

    int lowestLayer = getLowestLayer(aScene);
    int highestLayer = getHighestLayer(aScene);

    UIObject uiObject;

    for (int layer = lowestLayer; layer <= highestLayer; ++layer) {
        renderLayer(aScene, layer);
    }

    renderDebugInfo(aScene);

    mRenderer->show();
}

void RenderSystem::renderDebugInfo(Scene* aScene) {
    if (Time::deltaTime == 0) {
        return;
    }

    Configuration& config = EngineBravo::getInstance().getConfiguration();

    if (config.getConfig(SHOW_FPS)) {
        int fps = 1.0f / Time::deltaTime;

        // Render FPS counter in the top left corner of the screen with black text color (0, 0, 0)
        mRenderer->renderText("FPS: " + std::to_string(fps), Vector2(10, 10), Color(0, 255, 0), 1.5, 1.5);
    }

    if (config.getConfig(SHOW_COLLIDERS)) {
        for (auto& gameObject : aScene->getGameObjects()) {
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

                    Color renderColor = Color(0, 0, 255);

                    if (!boxCollider->isActive()) {
                        renderColor = Color(252, 3, 252);
                    }

                    mRenderer->renderSquare(drawPosition, spriteWidth, spriteHeight, renderColor, false);
                }
            }
        }
    }
}

Renderer& RenderSystem::getRenderer() { return *mRenderer; }
Window& RenderSystem::getWindow() { return *mWindow; }

void RenderSystem::addObject(GameObject& aObject) {
    auto it =
        std::find_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& wrapper) {
            return &wrapper.get() == &aObject; // Compare addresses
        });
    if (it == mObjects.end()) {
        mObjects.push_back(aObject);
    }
}

void RenderSystem::removeObject(GameObject& aObject) {
    auto it =
        std::remove_if(mObjects.begin(), mObjects.end(),
                       [&aObject](const std::reference_wrapper<GameObject>& obj) { return &obj.get() == &aObject; });
    if (it != mObjects.end()) {
        mObjects.erase(it, mObjects.end());
    }
}
