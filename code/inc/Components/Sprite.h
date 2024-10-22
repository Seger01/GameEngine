#pragma once

#include "Component.h"
#include <string>

class Sprite : public Component {
public:
    Sprite();
    ~Sprite();
    void render();
    void setFlipX(bool state) { mFlipX = state; }
    bool getFlipX() { return mFlipX; }

private:
    std::string mSprite;
    //??? mColor;
    //??? mColor;
    bool mFlipX;
    bool mFlipY;
    //??? mSortingLayer;
    //??? mOrderInLayer;
};
