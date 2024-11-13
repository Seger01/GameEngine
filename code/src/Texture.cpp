#include "Texture.h"
#include "Renderer.h"

// Texture::Texture(SDL_Renderer*& aRenderer) { this->mRenderer = aRenderer; }
Texture::Texture(SDL_Texture* aTexture, int aID) : mTexture(aTexture), mID(aID) {}

Texture::~Texture() {}

SDL_Texture* Texture::getSDLTexture() { return mTexture; }
