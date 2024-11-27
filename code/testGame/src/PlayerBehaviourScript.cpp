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

std::string PlayerBehaviourScript::currentActiveAnimationTag() {
    for (auto animation : mGameObject->getComponents<Animation>()) {
        if (animation->isActive()) {
            return animation->getTag();
        }
    }
    return "";
}

bool PlayerBehaviourScript::flipX() {
    if (mGameObject->hasComponent<Animation>()) {
        for (auto animation : mGameObject->getComponents<Animation>()) {
            return animation->getFlipX();
        }
    } else if (mGameObject->hasComponent<Sprite>()) {
        for (auto sprite : mGameObject->getComponents<Sprite>()) {
            return sprite->getFlipX();
        }
    }
    return false;
}

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

void PlayerBehaviourScript::onStart() {
	if (!isOwner())
	{
		destroy();
	}
}

void PlayerBehaviourScript::handleAnimations() {
    if (mGameObject->hasComponent<NetworkObject>()) {
        NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
        if (!networkObject) {
            return;
        }
        if (!networkObject->isOwner()) {
            return;
        }
    }
    static Transform previousTransform = this->mGameObject->getTransform();
    Transform currentTransform = this->mGameObject->getTransform();

    if (abs(previousTransform.position.x - currentTransform.position.x) < 0.5 &&
        abs(previousTransform.position.y - currentTransform.position.y) < 0.5) {
        if ((currentActiveAnimationTag() == "playerIdleFront") || (currentActiveAnimationTag() == "playerIdleBack") ||
            (currentActiveAnimationTag() == "playerIdleSide")) {
        } else {
            if (currentActiveAnimationTag() == "playerWalkingFront") {
                deactivateAllAnimations();
                setAnimationActive("playerIdleFront", true);
            } else if (currentActiveAnimationTag() == "playerWalkingBack") {
                deactivateAllAnimations();
                setAnimationActive("playerIdleBack", true);
            } else if (currentActiveAnimationTag() == "playerWalkingFrontSide") {
                deactivateAllAnimations();
                setAnimationActive("playerIdleSide", true);
            } else if (currentActiveAnimationTag() == "playerWalkingBackSide") {
                deactivateAllAnimations();
                setAnimationActive("playerIdleBack", true);
            } else {
                deactivateAllAnimations();
                setAnimationActive("playerIdleBack", true);
            }

            setFlipX(!flipX());
        }

        if (abs(previousTransform.position.x - currentTransform.position.x) > 0.3) {
            if (previousTransform.position.x > currentTransform.position.x) {
                setFlipX(true);
            } else if (currentTransform.position.x > previousTransform.position.x) {
                setFlipX(false);
            }
        }

    } else {
        if (currentTransform.position.y < previousTransform.position.y) {
            if (abs(currentTransform.position.x - previousTransform.position.x) > 0.5) {
                deactivateAllAnimations();
                setAnimationActive("playerWalkingBackSide", true);
            } else if (abs(currentTransform.position.x - previousTransform.position.x) < 0.5) {
                deactivateAllAnimations();
                setAnimationActive("playerWalkingBack", true);
            }
        } else {
            if (abs(currentTransform.position.x - previousTransform.position.x) > 0.5) {
                deactivateAllAnimations();
                setAnimationActive("playerWalkingFrontSide", true);
            } else if (abs(currentTransform.position.x - previousTransform.position.x) < 0.5) {
                deactivateAllAnimations();
                setAnimationActive("playerWalkingFront", true);
            }
        }

        if ((currentTransform.position.x - previousTransform.position.x) < -0.5) {
            setFlipX(false);
        } else if ((currentTransform.position.x - previousTransform.position.x) > 0.5) {
            setFlipX(true);
        }
    }

    previousTransform = this->mGameObject->getTransform();
}

void PlayerBehaviourScript::handleMovement() {
    static const float movementSpeed = 50.0f;

    if (mGameObject->hasComponent<NetworkObject>()) {
        NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
        if (!networkObject) {
            return;
        }
        if (!networkObject->isOwner()) {
            return;
        }
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
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(0, -200));
    }
    if (input.GetKey(Key::Key_A)) {
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(-200, 0));
    }
    if (input.GetKey(Key::Key_S)) {
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(0, 200));
    }
    if (input.GetKey(Key::Key_D)) {
        mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(200, 0));
    }
    this->mGameObject->setTransform(parentTransform);
}

void PlayerBehaviourScript::hanldeCameraMovement() {
    if (mGameObject->hasComponent<NetworkObject>()) {
        NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
        if (!networkObject) {
            return;
        }
        if (!networkObject->isOwner()) {
            return;
        }
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
