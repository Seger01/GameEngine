#include "PlayerBehaviourScript.h"

#include <iostream>

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "Input.h"
#include "InputStructs.h"
#include "Network/NetworkObject.h"
#include "ParticleEmitter.h"
#include "Sprite.h"
#include "SpriteDef.h"
#include "SpriteDefUtil.h"

const int spriteWidth = 16;  // Width of each sprite
const int spriteHeight = 25; // Height of each sprite

const Point playerIdleBackPosition = {21, 95};
SpriteDef firstFramePlayerIdleBack = {
    "enter_the_gungeon_spritesheet.png",
    Rect{playerIdleBackPosition.x, playerIdleBackPosition.y, spriteWidth, spriteHeight}, spriteWidth, spriteHeight};

Point playerIdleSidePosition = {21, 66};
SpriteDef firstFramePlayerIdleSize = {
    "enter_the_gungeon_spritesheet.png",
    Rect{playerIdleSidePosition.x, playerIdleSidePosition.y, spriteWidth, spriteHeight}, spriteWidth, spriteHeight};

const Point playerIdleFrontPosition = {21, 37};
SpriteDef firstFramePlayerIdleFront = {
    "enter_the_gungeon_spritesheet.png",
    Rect{playerIdleFrontPosition.x, playerIdleFrontPosition.y, spriteWidth, spriteHeight}, spriteWidth, spriteHeight};

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

ParticleEmitter* emitter = nullptr;

void PlayerBehaviourScript::deactivateAllAnimations() {
    for (auto animation : mGameObject->getComponents<Animation>()) {
        animation->setActive(false);
    }
}

void PlayerBehaviourScript::setAnimationActive(std::string aAnimationTag, bool aState) {
    for (auto animation : mGameObject->getComponents<Animation>()) {
        if (animation->getTag() == aAnimationTag) {
            animation->setActive(aState);
        }
    }
}

void PlayerBehaviourScript::initEmitter() {

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

    std::vector<Color> colors = {Color(255, 49, 3, 170), Color(255, 100, 3, 150), Color(0, 0, 0, 90),
                                 Color(0, 0, 0, 90)};
    // std::vector<Color> colors = {Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255),
    //                              Color(0, 0, 255), Color(0, 255, 0), Color(255, 0, 0)};

    emitter = new ParticleEmitter(emitterMode, speed, acceleration, minLifeTimeMs, maxLifeTimeMs, startSize, endSize,
                                  rotation, rotationSpeed, rotationAcceleration, colors);

    emitter->setParticlesPerSecond(300);
    emitter->setAngle(0, 45);
    emitter->setLayer(4);
    mGameObject->addComponent(emitter);
}

void PlayerBehaviourScript::onStart() {
    std::cout << "on start called" << std::endl;
    Animation* playerIdleFrontAnimation = nullptr;
    Animation* playerIdleSideAnimation = nullptr;
    Animation* playerIdleBackAnimation = nullptr;

    {
        std::vector<SpriteDef> playerIdleBackAnimationFrames =
            SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleBack, 3);

        std::vector<SpriteDef> playerAnimationIdleBack = {
            playerIdleBackAnimationFrames[0], playerIdleBackAnimationFrames[1], playerIdleBackAnimationFrames[2],
            playerIdleBackAnimationFrames[1]};

        playerIdleBackAnimation =
            EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleBack, 200, true);
    }

    {
        std::vector<SpriteDef> playerIdleSideAnimationFrames =
            SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleSize, 3);

        std::vector<SpriteDef> playerAnimationIdleSide = {
            playerIdleSideAnimationFrames[0], playerIdleSideAnimationFrames[1], playerIdleSideAnimationFrames[2],
            playerIdleSideAnimationFrames[1]};

        playerIdleSideAnimation =
            EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleSide, 200, true);
    }
    {
        std::vector<SpriteDef> playerIdleFrontAnimationFrames =
            SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleFront, 3);

        std::vector<SpriteDef> playerAnimaionIdleFront = {
            playerIdleFrontAnimationFrames[0], playerIdleFrontAnimationFrames[1], playerIdleFrontAnimationFrames[2],
            playerIdleFrontAnimationFrames[1]};

        playerIdleFrontAnimation =
            EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimaionIdleFront, 200, true);
    }

    mGameObject->addComponent(playerIdleFrontAnimation);
    mGameObject->addComponent(playerIdleSideAnimation);
    mGameObject->addComponent(playerIdleBackAnimation);

    for (auto animation : mGameObject->getComponents<Animation>()) {
        animation->setActive(false);
        animation->setLayer(3);
    }

    playerIdleFrontAnimation->setActive(true);

    playerIdleBackAnimation->setTag("playerIdleBack");
    playerIdleSideAnimation->setTag("playerIdleSide");
    playerIdleFrontAnimation->setTag("playerIdleFront");

    initEmitter();
}

void PlayerBehaviourScript::handleAnimations() {
    NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
    if (!networkObject) {
        return;
    }
    if (!networkObject->isOwner()) {
        return;
    }
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

    NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
    if (!networkObject) {
        return;
    }
    if (!networkObject->isOwner()) {
        return;
    }

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
        setAnimationActive("playerIdleBack", true);
        setFlipX(false);
        parentTransform.position.y -= (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_A)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleSide", true);
        setFlipX(true);
        parentTransform.position.x -= (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_S)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleFront", true);
        setFlipX(false);
        parentTransform.position.y += (movementSpeed * Time::deltaTime);
    }
    if (input.GetKey(Key::Key_D)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleSide", true);
        setFlipX(false);
        parentTransform.position.x += (movementSpeed * Time::deltaTime);
    }
    this->mGameObject->setTransform(parentTransform);
}

void PlayerBehaviourScript::hanldeCameraMovement() {
    NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
    if (!networkObject) {
        return;
    }
    if (!networkObject->isOwner()) {
        return;
    }
    Camera& currentCam = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getActiveCamera();

    Transform playerTransform = this->mGameObject->getTransform();

    currentCam.setTransform(playerTransform);
}

void PlayerBehaviourScript::onUpdate() {
    Input& input = Input::getInstance();

    handleMovement();
    handleAnimations();

    hanldeCameraMovement();

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

    if (input.GetKeyDown(Key::Key_C)) {
        Configuration& config = EngineBravo::getInstance().getConfiguration();
        config.setConfig("render_colliders", !config.getConfig("render_colliders"));
    }

    if (input.GetKeyDown(Key::Key_F)) {
        Configuration& config = EngineBravo::getInstance().getConfiguration();
        config.setConfig("render_fps", !config.getConfig("render_fps"));
    }

    if (emitter != nullptr) {
        static bool emitterMode = false;

        if (input.GetKeyDown(Key::Key_P)) {
            emitter->setActive(!emitter->isActive());
        }

        if (input.GetKeyDown(Key::Key_Space)) {
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
}
