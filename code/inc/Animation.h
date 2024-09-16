#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

// class Texture;
#include "Rectangle.h"
#include "Texture.h"
// struct spriteCoords {
//     int spriteStart = 0;
//     int spriteLength = 0;
//     int spriteWidth = 0;
// };

class Animation {
public:
    Animation();
    Animation(Texture*& texture, Rectangle aStartingFrame, int aAmountOfFrames);
    ~Animation();

    Texture*& getTexture();
    Rectangle& getCurrentFrame();

private:
    std::vector<Rectangle> sprites = {};
    Texture* mSourceTexture = nullptr;

    Rectangle currentFrame;
    int animationSpeed = 200;
};

#endif
