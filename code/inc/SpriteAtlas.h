#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Animation.h"

class SpriteAtlas {
public:
    SpriteAtlas(SDL_Renderer*& renderer);
    SpriteAtlas(SDL_Renderer*& renderer, std::string filePath);
    ~SpriteAtlas();

    Animation& getAnimation(SDL_Rect aStartingFrame, int aAmountOfFrames);

    void loadTexture(std::string aFilePath);

private:
    void unloadTexture();

    SDL_Texture* loadedTexture = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool textureIsLoaded = false;

    std::vector<Animation> mCreatedAnimations = {};
};
