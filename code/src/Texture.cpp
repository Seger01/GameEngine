#include "Texture.h"
#include "Renderer.h"
// Texture::Texture(SDL_Renderer*& aRenderer) { this->mRenderer = aRenderer; }
Texture::Texture(Renderer*& aRenderer) { this->mRenderer = aRenderer; }

Texture::~Texture() {}

void Texture::load(std::string aFilePath) {
    // Load image
    SDL_Surface* tempSurface = IMG_Load(aFilePath.c_str());
    if (tempSurface == NULL) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return;
    }

    mLoadedTexture = SDL_CreateTextureFromSurface(mRenderer->getSDLRenderer(), tempSurface);
    SDL_FreeSurface(tempSurface);

    mTextureLoaded = true;

    return;
}

SDL_Texture*& Texture::getSDL_Texture() { return this->mLoadedTexture; }

// void Texture::render(
