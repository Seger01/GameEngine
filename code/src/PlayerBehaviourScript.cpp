#include "PlayerBehaviourScript.h"

#include <iostream>

#include "Animation.h"
#include "GameObject.h"
#include "Input.h"
#include "Sprite.h"

void PlayerBehaviourScript::setFlipX(bool aState) {
    if (mGameObject->hasComponent<Animation>()) {
        Animation* animation = nullptr;
        animation = mGameObject->getComponent<Animation>();
        if (animation == nullptr) {
            return;
        }
        animation->setFlipX(aState);
    } else if (mGameObject->hasComponent<Sprite>()) {
        Sprite* sprite = nullptr;
        sprite = mGameObject->getComponent<Sprite>();
        if (sprite == nullptr) {
            return;
        }
        sprite->setFlipX(aState);
    }
}
void PlayerBehaviourScript::setFlipY(bool aState) {
    if (mGameObject->hasComponent<Animation>()) {
        Animation* animation = nullptr;
        animation = mGameObject->getComponent<Animation>();
        if (animation == nullptr) {
            return;
        }
        animation->setFlipY(aState);
    } else if (mGameObject->hasComponent<Sprite>()) {
        Sprite* sprite = nullptr;
        sprite = mGameObject->getComponent<Sprite>();
        if (sprite == nullptr) {
            return;
        }
        sprite->setFlipY(aState);
    }
}

void PlayerBehaviourScript::onStart() {}
void PlayerBehaviourScript::onUpdate() {
    static const float movementSpeed = 15.0f;

    std::cout << "PlayerBehaviourScript::onUpdate()" << std::endl;

    Input& input = Input::getInstance();

    Transform parentTransform = this->mGameObject->getTransform();

    if (input.GetKey(Key::Key_R)) {
        parentTransform.rotate(1.0f);
    }
    if (input.GetKey(Key::Key_T)) {
        parentTransform.rotate(-1.0f);
    }

    if (input.GetKey(Key::Key_W)) {
        parentTransform.position.y -= movementSpeed;
        setFlipY(false);
    }
    if (input.GetKey(Key::Key_A)) {
        parentTransform.position.x -= movementSpeed;
        setFlipX(false);
    }
    if (input.GetKey(Key::Key_S)) {
        parentTransform.position.y += movementSpeed;
        setFlipY(true);
    }
    if (input.GetKey(Key::Key_D)) {
        parentTransform.position.x += movementSpeed;
        setFlipX(true);
    }
    this->mGameObject->setTransform(parentTransform);
}
