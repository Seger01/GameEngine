#include "PlayerBehaviourScript.h"

#include <iostream>

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "Input.h"
#include "ParticleEmitter.h"
#include "Sprite.h"
#include "SpriteDef.h"

void PlayerBehaviourScript::setFlipX(bool aState) {
    if (mGameObject->hasComponent<Animation>()) {
        for (auto animation : mGameObject->getComponents<Animation>()) {
            animation->setFlipX(aState);
        }
    } else if (mGameObject->hasComponent<Sprite>()) {
        for (auto sprite : mGameObject->getComponents<Sprite>()) {
            sprite->setFlipX(aState);
        }
    }
}
void PlayerBehaviourScript::setFlipY(bool aState) {
    if (mGameObject->hasComponent<Animation>()) {
        for (auto animation : mGameObject->getComponents<Animation>()) {
            animation->setFlipY(aState);
        }
    } else if (mGameObject->hasComponent<Sprite>()) {
        for (auto sprite : mGameObject->getComponents<Sprite>()) {
            sprite->setFlipY(aState);
        }
    }
}

void PlayerBehaviourScript::toggleAnimaionEnabled() {
    if (mGameObject->hasComponent<Animation>()) {
        for (auto animation : mGameObject->getComponents<Animation>()) {
            animation->setActive(!animation->isActive());
        }
    }
}

Animation* playerIdleFrontAnimation = nullptr;
Animation* playerIdleSideAnimation = nullptr;
Animation* playerIdleBackAnimation = nullptr;

ParticleEmitter* emitter = nullptr;

std::vector<Animation*> playerAnimations = {playerIdleFrontAnimation, playerIdleSideAnimation, playerIdleBackAnimation};

void PlayerBehaviourScript::deactivateAllAnimations() {
    // for (auto animation : playerAnimations) {
    //     if (animation != nullptr) {
    //         animation->setActive(false);
    //     }
    // }
    if (playerIdleFrontAnimation != nullptr) {
        playerIdleFrontAnimation->setActive(false);
    }
    if (playerIdleSideAnimation != nullptr) {
        playerIdleSideAnimation->setActive(false);
    }
    if (playerIdleBackAnimation != nullptr) {
        playerIdleBackAnimation->setActive(false);
    }
}

void PlayerBehaviourScript::setAnimationActive(Animation* aAnimation, bool aState) {
    if (aAnimation != nullptr) {
        aAnimation->setActive(aState);
    }
}

void PlayerBehaviourScript::onStart() {
    std::cout << "PlayerBehaviourScript::onStart()" << std::endl;

    const int sizeMultiplier = 1;

    const int spriteWidth = 16;  // Width of each sprite
    const int spriteHeight = 25; // Height of each sprite
    const int frameCount = 6;    // Total number of frames in the sprite sheet

    // SDL_Texture* spriteSheetTexture = loadTexture(renderer, "enter_the_gungeon_spritesheet.png");
    // SpriteAtlas spriteAtlas(myRenderer, "enter_the_gungeon_spritesheet.png");

    // Rectangle startOfAnimation;
    // startOfAnimation.x = 22;           // Move horizontally in the sprite sheet
    // startOfAnimation.y = 187;          // Keep the vertical position constant (you can change this for vertical
    // movement) startOfAnimation.w = spriteWidth;  // The width of the sprite startOfAnimation.h = spriteHeight; // The
    // height of the sprite

    // SpriteDef playerAnimationSprite1 = {"enter_the_gungeon_spritesheet.png", Rect{22, 187, spriteWidth,
    // spriteHeight},
    //                                     16 * 5, 25 * 5};
    // SpriteDef playerAnimationSprite2 = {"enter_the_gungeon_spritesheet.png",
    //                                     Rect{22 + (spriteWidth * 1), 187, spriteWidth, spriteHeight}, 16 * 5, 25 *
    //                                     5};
    // SpriteDef playerAnimationSprite3 = {"enter_the_gungeon_spritesheet.png",
    //                                     Rect{22 + (spriteWidth * 2), 187, spriteWidth, spriteHeight}, 16 * 5, 25 *
    //                                     5};
    // SpriteDef playerAnimationSprite4 = {"enter_the_gungeon_spritesheet.png",
    //                                     Rect{22 + (spriteWidth * 3), 187, spriteWidth, spriteHeight}, 16 * 5, 25 *
    //                                     5};
    // SpriteDef playerAnimationSprite5 = {"enter_the_gungeon_spritesheet.png",
    //                                     Rect{22 + (spriteWidth * 4), 187, spriteWidth, spriteHeight}, 16 * 5, 25 *
    //                                     5};
    // SpriteDef playerAnimationSprite6 = {"enter_the_gungeon_spritesheet.png",
    //                                     Rect{22 + (spriteWidth * 5), 187, spriteWidth, spriteHeight}, 16 * 5, 25 *
    //                                     5};
    //
    // std::vector<SpriteDef> playerAnimationJump = {playerAnimationSprite1, playerAnimationSprite2,
    //                                               playerAnimationSprite3, playerAnimationSprite4,
    //                                               playerAnimationSprite5, playerAnimationSprite6};
    {
        const int spriteWidth = 16;  // Width of each sprite
        const int spriteHeight = 25; // Height of each sprite

        const int AnimationStartInSpriteSheetX = 21;
        const int AnimationStartInSpriteSheetY = 95;

        SpriteDef playerFrameIdle1 = {
            "enter_the_gungeon_spritesheet.png",
            Rect{AnimationStartInSpriteSheetX, AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
            spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        SpriteDef playerFrameIdle2 = {"enter_the_gungeon_spritesheet.png",
                                      Rect{AnimationStartInSpriteSheetX + (1 * spriteWidth),
                                           AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
                                      spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        SpriteDef playerFrameIdle3 = {"enter_the_gungeon_spritesheet.png",
                                      Rect{AnimationStartInSpriteSheetX + (2 * spriteWidth),
                                           AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
                                      spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        std::vector<SpriteDef> playerAnimationIdleBack = {playerFrameIdle1, playerFrameIdle2, playerFrameIdle3,
                                                          playerFrameIdle2};

        playerIdleBackAnimation =
            EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleBack, 200, true);
    }

    {
        const int spriteWidth = 16;  // Width of each sprite
        const int spriteHeight = 25; // Height of each sprite

        const int AnimationStartInSpriteSheetX = 21;
        const int AnimationStartInSpriteSheetY = 66;

        SpriteDef playerFrameIdle1 = {
            "enter_the_gungeon_spritesheet.png",
            Rect{AnimationStartInSpriteSheetX, AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
            spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        SpriteDef playerFrameIdle2 = {"enter_the_gungeon_spritesheet.png",
                                      Rect{AnimationStartInSpriteSheetX + (1 * spriteWidth),
                                           AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
                                      spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        SpriteDef playerFrameIdle3 = {"enter_the_gungeon_spritesheet.png",
                                      Rect{AnimationStartInSpriteSheetX + (2 * spriteWidth),
                                           AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
                                      spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        std::vector<SpriteDef> playerAnimationIdleSide = {playerFrameIdle1, playerFrameIdle2, playerFrameIdle3,
                                                          playerFrameIdle2};

        playerIdleSideAnimation =
            EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleSide, 200, true);
    }
    {
        const int spriteWidth = 16;  // Width of each sprite
        const int spriteHeight = 25; // Height of each sprite

        const int AnimationStartInSpriteSheetX = 21;
        const int AnimationStartInSpriteSheetY = 37;

        SpriteDef playerFrameIdle1 = {
            "enter_the_gungeon_spritesheet.png",
            Rect{AnimationStartInSpriteSheetX, AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
            spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        SpriteDef playerFrameIdle2 = {"enter_the_gungeon_spritesheet.png",
                                      Rect{AnimationStartInSpriteSheetX + (1 * spriteWidth),
                                           AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
                                      spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        SpriteDef playerFrameIdle3 = {"enter_the_gungeon_spritesheet.png",
                                      Rect{AnimationStartInSpriteSheetX + (2 * spriteWidth),
                                           AnimationStartInSpriteSheetY, spriteWidth, spriteHeight},
                                      spriteWidth * sizeMultiplier, spriteHeight * sizeMultiplier};

        std::vector<SpriteDef> playerAnimaionIdleFront = {playerFrameIdle1, playerFrameIdle2, playerFrameIdle3,
                                                          playerFrameIdle2};

        playerIdleFrontAnimation =
            EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimaionIdleFront, 200, true);
    }

    playerIdleBackAnimation->setActive(false);
    playerIdleSideAnimation->setActive(false);
    playerIdleFrontAnimation->setActive(true);

    playerIdleFrontAnimation->setLayer(1);
    playerIdleSideAnimation->setLayer(1);
    playerIdleBackAnimation->setLayer(1);

    mGameObject->addComponent(playerIdleFrontAnimation);
    mGameObject->addComponent(playerIdleSideAnimation);

    EmitterMode emitterMode = EmitterMode::Continuous;
    float speed = 50.0f;
    float acceleration = 0.0f;
    int minLifeTimeMs = 100;
    int maxLifeTimeMs = 1000;
    Vector2 startSize = Vector2(5, 5);

    Vector2 endSize = Vector2(0, 0);

    float rotation = 0.0f;
    float rotationSpeed = 0.0f;
    float rotationAcceleration = 0.0f;

    std::vector<Color> colors = {Color(255, 49, 3), Color(255, 100, 3), Color(0, 0, 0), Color(0, 0, 0)};
    // std::vector<Color> colors = {Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255),
    //                              Color(0, 0, 255), Color(0, 255, 0), Color(255, 0, 0)};

    emitter = new ParticleEmitter(emitterMode, speed, acceleration, minLifeTimeMs, maxLifeTimeMs, startSize, endSize,
                                  rotation, rotationSpeed, rotationAcceleration, colors);

    mGameObject->addComponent(playerIdleBackAnimation);

    emitter->setParticlesPerSecond(100);
    emitter->setAngle(0, 45);
    mGameObject->addComponent(emitter);

    // emitter->getRelativeTransform().position.y += static_cast<int>((spriteHeight * sizeMultiplier) / 2);
    // emitter->getRelativeTransform().position.x += static_cast<int>((spriteWidth * sizeMultiplier) / 2);
}

void PlayerBehaviourScript::handleAnimations() {
    static Transform previousTransform = this->mGameObject->getTransform();

    // deactivateAllAnimations();
    // setAnimationActive(playerIdleFrontAnimation, true);
    // if (previousTransform.position.x < this->mGameObject->getTransform().position.x) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleSideAnimation, true);
    //     setFlipX(false);
    // } else if (previousTransform.position.x > this->mGameObject->getTransform().position.x) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleSideAnimation, true);
    //     setFlipX(true);
    // } else if (previousTransform.position.y < this->mGameObject->getTransform().position.y) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleBackAnimation, true);
    //     setFlipX(false);
    // } else if (previousTransform.position.y > this->mGameObject->getTransform().position.y) {
    //     deactivateAllAnimations();
    //     setAnimationActive(playerIdleFrontAnimation, true);
    //     setFlipX(false);
    // }

    previousTransform = this->mGameObject->getTransform();
}

void PlayerBehaviourScript::handleMovement() {
    static const float movementSpeed = 50.0f;

    Input& input = Input::getInstance();

    Transform parentTransform = this->mGameObject->getTransform();

    if (input.GetKey(Key::Key_E)) {
        deactivateAllAnimations();
    }

    if (input.GetKey(Key::Key_R)) {
        parentTransform.rotate(1.0f);
    }
    if (input.GetKey(Key::Key_T)) {
        parentTransform.rotate(-1.0f);
    }

    if (input.GetKey(Key::Key_W)) {
        deactivateAllAnimations();
        setAnimationActive(playerIdleBackAnimation, true);
        setFlipX(false);
        parentTransform.position.y -= (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_A)) {
        deactivateAllAnimations();
        setAnimationActive(playerIdleSideAnimation, true);
        setFlipX(true);
        parentTransform.position.x -= (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_S)) {
        deactivateAllAnimations();
        setAnimationActive(playerIdleFrontAnimation, true);
        setFlipX(false);
        parentTransform.position.y += (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_D)) {
        deactivateAllAnimations();
        setAnimationActive(playerIdleSideAnimation, true);
        setFlipX(false);
        parentTransform.position.x += (movementSpeed * Time::deltaTime);
    }
    this->mGameObject->setTransform(parentTransform);
}

void PlayerBehaviourScript::onUpdate() {
    handleMovement();
    handleAnimations();

    // Camera& currentCam = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getActiveCamera();
    //
    // static bool direction = true;
    //
    // if (direction) {
    //     Transform pos = currentCam.getTransform();
    //
    //     pos.position.x += 0.01f * Time::deltaTime;
    //
    //     currentCam.setTransform(pos);
    // } else {
    //     Transform pos = currentCam.getTransform();
    //
    //     pos.position.x -= 0.01f * Time::deltaTime;
    //
    //     currentCam.setTransform(pos);
    // }
    //
    // if (currentCam.getTransform().position.x > 100) {
    //     direction = false;
    // } else if (currentCam.getTransform().position.x < 40) {
    //     direction = true;
    // }

    // emitter->setActive(false);

    static bool emitterMode = false;

    if (Input::getInstance().GetKeyDown(Key::Key_Space)) {
        emitterMode = !emitterMode;
    }

    if (emitterMode) {
        emitter->setAngle(0, 45);
        emitter->getRelativeTransform().rotation += 1.0f * Time::deltaTime;
    } else {
        emitter->setAngle(0, 360);
        emitter->getRelativeTransform().rotation = 0.0f;
    }
}
