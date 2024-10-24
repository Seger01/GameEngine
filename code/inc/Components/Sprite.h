#pragma once

#include <string>

#include "Component.h"
#include "Texture.h"

class Sprite : public Component {
public:
    Sprite(Texture* aTexture);
    ~Sprite();

    void setFlipX(bool state) { mFlipX = state; }
    bool getFlipX() { return mFlipX; }

    Texture* getTexture();

private:
    Texture* mTexture = nullptr;

    std::string mSprite;
    //??? mColor;
    //??? mColor;
    bool mFlipX;
    bool mFlipY;
    //??? mSortingLayer;
    //??? mOrderInLayer;
};
