#pragma once

#include "Component.h"
#include <string>

class Sprite : public Component
{
    public:
        Sprite();
        ~Sprite();
        void render();
    private:
        std::string mSprite;
        //??? mColor;
        //??? mColor;
        bool mFlipX;
        bool mFlipY;
        //??? mSortingLayer;
        //??? mOrderInLayer;
};