#ifndef SPRITEATLAS_H
#define SPRITEATLAS_H

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "Animation.h"
#include "Rectangle.h"
#include "Texture.h"

class SpriteAtlas {
public:
    // SpriteAtlas(SDL_Renderer*& renderer);
    // SpriteAtlas(SDL_Renderer*& renderer, std::string filePath);
    SpriteAtlas(Renderer*& aRenderer, std::string aFilePath);
    ~SpriteAtlas();

    Animation& getAnimation(Rectangle aStartingFrame, int aAmountOfFrames);

    void loadTexture(std::string aFilePath);

private:
    void unloadTexture();

    Texture* mTexture = nullptr;

    Renderer* mRenderer = nullptr;

    bool textureIsLoaded = false;

    std::vector<Animation> mCreatedAnimations = {};
};

#endif
