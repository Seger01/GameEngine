#include "Components/Sprite.h"
#include <iostream>

#include "GameObject.h"

Sprite::Sprite(Texture* aTexture) : mTexture(aTexture), mFlipX(false), mFlipY(false) {}

Sprite::~Sprite() {
    // Cleanup resources if necessary
}
Texture* Sprite::getTexture() { return mTexture; }

Transform Sprite::getTransform() {
    Transform& parentTransform = this->mGameObject->getTransform();

    Transform returnTransform = parentTransform;

    // returnTransform = returnTransform + mTransform;
    returnTransform += mTransform;

    return returnTransform;
}

Transform* Sprite::getTransformPtr() { return &mTransform; }
