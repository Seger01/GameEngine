#include "Components/Sprite.h"

Sprite::Sprite(Texture* aTexture, int aWidth, int aHeight, Rect aSourceRect) {
    mTexture = aTexture;
    mWidth = aWidth;
    mHeight = aHeight;
    mSourceRect = aSourceRect;
}

Sprite::~Sprite() {
    // Cleanup resources if necessary
}
Texture* Sprite::getTexture() { return mTexture; }

Transform Sprite::getRelativePosition() { return mTransform; }
void Sprite::setRelativePosition(Transform aNewTransform) { mTransform = aNewTransform; }

Rect Sprite::getSource() { return mSourceRect; }

void Sprite::setSource(Rect aSourceRect) { mSourceRect = aSourceRect; }
