#include "PlayerBehaviourScript.h"

#include <iostream>

#include "Animation.h"
#include "BulletBehaviourScript.h"
#include "BulletPrefab.h"
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
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(0, 200));
    }
    if (input.GetKey(Key::Key_A)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleSide", true);
        setFlipX(true);
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(200, 0));
    }
    if (input.GetKey(Key::Key_S)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleFront", true);
        setFlipX(false);
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(0, -200));
    }
    if (input.GetKey(Key::Key_D)) {
        deactivateAllAnimations();
        setAnimationActive("playerIdleSide", true);
        setFlipX(false);
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(-200, 0));
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

void PlayerBehaviourScript::fireBullet() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    Input& input = Input::getInstance();

    // Get the mouse position in screen space
    Point mousePosition = input.MousePosition();
    Vector2 mousePositionVector = Vector2(mousePosition.x, mousePosition.y);

    // Get the camera
    Camera& currentCam = sceneManager.getCurrentScene()->getActiveCamera();
    Vector2 cameraOrigin = currentCam.getOrigin();

    // Transform the mouse position to world space
    Vector2 screenCenter(currentCam.getWidth() / 2.0f, currentCam.getHeight() / 2.0f);
    Vector2 relativeMousePosition = mousePositionVector - screenCenter;

    // Transform to world space
    Vector2 worldMousePosition = cameraOrigin + relativeMousePosition;

    // Calculate the direction from the player to the mouse position
    Vector2 playerPosition = this->mGameObject->getTransform().position;
    Vector2 direction = worldMousePosition - playerPosition;

    // std::cout << "Screen Mouse: (" << mousePosition.x << ", " << mousePosition.y << ")" << std::endl;
    // std::cout << "Screen Center: (" << screenCenter.x << ", " << screenCenter.y << ")" << std::endl;
    // std::cout << "Relative Mouse: (" << relativeMousePosition.x << ", " << relativeMousePosition.y << ")" <<
    // std::endl; std::cout << "World Mouse: (" << worldMousePosition.x << ", " << worldMousePosition.y << ")" <<
    // std::endl; std::cout << "Player Pos: (" << playerPosition.x << ", " << playerPosition.y << ")" << std::endl;
    // std::cout << "Direction before norm: (" << direction.x << ", " << direction.y << ")" << std::endl;

    // Normalize the direction vector
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0001f) {
        direction.x /= length;
        direction.y /= length;
    }

    direction = direction * -1;

    // std::cout << "Final Direction: (" << direction.x << ", " << direction.y << ")" << std::endl;

    // Create and setup the bullet
    GameObject* bulletObject = BulletPrefabFactory().createBulletPrefab(*this->mGameObject);
    RigidBody* bulletRigidBody = bulletObject->getComponents<RigidBody>()[0];
    bulletRigidBody->addForce(direction * 1000.0f);

    sceneManager.getCurrentScene()->addGameObject(bulletObject);
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

    if (input.GetMouseButtonDown(MouseButton::LEFT)) {
        fireBullet();
    }

    if (mGameObject->hasComponent<ParticleEmitter>()) {
        static bool emitterMode = false;

        ParticleEmitter* emitter = mGameObject->getComponents<ParticleEmitter>()[0];

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
    if (aGameObject != nullptr) {
        // std::cout << "Player collided with " << aGameObject->getName() << std::endl;
    }
}
