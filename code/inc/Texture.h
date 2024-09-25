#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.h"

class Texture {
public:
    // Texture(SDL_Renderer*& aRenderer);
    Texture(Renderer*& aRenderer);
    ~Texture();

    void load(std::string aFilePath);
    SDL_Texture*& getSDL_Texture();

private:
    SDL_Texture* mLoadedTexture = nullptr;

    // SDL_Renderer* mRenderer = nullptr;
    Renderer* mRenderer = nullptr;

    bool mTextureLoaded = false;
};

#endif
