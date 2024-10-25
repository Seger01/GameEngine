#pragma once

#include <string>

#include "Component.h"
#include "Texture.h"
#include "Transform.h"

class Sprite : public Component {
public:
    Sprite(Texture* aTexture);
    ~Sprite();

    Texture* getTexture();
    Transform getTransform();
    Transform* getTransformPtr();
    void setTransform(Transform aNewTransform);

    void setFlipX(bool state) { mFlipX = state; }
    bool getFlipX() { return mFlipX; }

    void setFlipY(bool state) { mFlipY = state; }
    bool getFlipY() { return mFlipY; }

    void setWidth(int aWidth) { mWidth = aWidth; }
    int getWidth() { return mWidth; }

    void setHeight(int aHeight) { mHeight = aHeight; }
    int getHeight() { return mHeight; }

private:
    Texture* mTexture = nullptr;
    Transform mTransform;

    std::string mSprite;
    int mWidth = 0;
    int mHeight = 0;
    //??? mColor;
    //??? mColor;
    bool mFlipX;
    bool mFlipY;
    //??? mSortingLayer;
    //??? mOrderInLayer;
};
