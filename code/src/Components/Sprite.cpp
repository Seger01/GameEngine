#include "Components/Sprite.h"
#include <iostream>

Sprite::Sprite(Texture* aTexture) : mTexture(aTexture), mFlipX(false), mFlipY(false) {}

Sprite::~Sprite() {
    // Cleanup resources if necessary
}
Texture* Sprite::getTexture() { return mTexture; }
