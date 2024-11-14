#include "PlayerBehaviourScript.h"

#include <iostream>

#include "Animation.h"
#include "Configuration.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "Input.h"
#include "InputStructs.h"
#include "Network/NetworkObject.h"
#include "ParticleEmitter.h"
#include "Sprite.h"
#include "SpriteDef.h"
#include "SpriteDefUtil.h"

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

void PlayerBehaviourScript::onStart() {}

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

    if (input.GetKeyDown(Key::Key_C)) {
        Configuration& config = EngineBravo::getInstance().getConfiguration();
        config.setConfig(SHOW_COLLIDERS, !config.getConfig(SHOW_COLLIDERS));
    }

    if (input.GetKeyDown(Key::Key_F)) {
        Configuration& config = EngineBravo::getInstance().getConfiguration();
        config.setConfig(SHOW_FPS, !config.getConfig(SHOW_FPS));
    }

    if (input.GetKeyDown(Key::Key_V)) {
        std::cout << "spawning new player" << std::endl;
        EngineBravo& engine = EngineBravo::getInstance();
        Scene* scene = engine.getSceneManager().getCurrentScene();

        GameObject* newPlayer = new GameObject(*mGameObject);

        Transform newPlayerTransform = newPlayer->getTransform();
        newPlayerTransform.position.x += 10;
        newPlayer->setTransform(newPlayerTransform);

        std::cout << "New player Tag: " << newPlayer->getTag() << std::endl;
        std::cout << "New player ID: " << newPlayer->getID() << std::endl;
        std::cout << "New player Name: " << newPlayer->getName() << std::endl;
        std::cout << "New player Active: " << newPlayer->isActive() << std::endl;
        std::cout << "New player Transform: " << newPlayer->getTransform().position.x << std::endl;
        std::cout << "New player Parent: " << newPlayer->getParent() << std::endl;

        for (auto& component : newPlayer->getComponents<Animation>()) {
            std::cout << "New player Animation Tag: " << component->getTag() << std::endl;
            std::cout << "New player Animation Active: " << component->isActive() << std::endl;
        }

        scene->addGameObject(newPlayer);
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

void PlayerBehaviourScript::onCollide(GameObject* aGameObject) {
    std::cout << "Player collided with " << aGameObject->getName() << std::endl;
}