#pragma once

#include <iostream>
#include <vector>

#include <SDL.h>

// struct spriteCoords {
//     int spriteStart = 0;
//     int spriteLength = 0;
//     int spriteWidth = 0;
// };

class Animation {
public:
    Animation();
    Animation(SDL_Texture*& texture, SDL_Rect aStartingFrame, int aAmountOfFrames);
    ~Animation();

    SDL_Texture*& getTexture();
    SDL_Rect& getCurrentFrame();

private:
    std::vector<SDL_Rect> sprites = {};
    SDL_Texture* sourceTexture = nullptr;

    SDL_Rect currentFrame;
    int animationSpeed = 200;
};
