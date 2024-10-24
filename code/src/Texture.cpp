#include "Texture.h"
#include "Renderer.h"
// Texture::Texture(SDL_Renderer*& aRenderer) { this->mRenderer = aRenderer; }
Texture::Texture(SDL_Texture* aTexture) { mTexture = aTexture; }

Texture::~Texture() {}

SDL_Texture* Texture::getSDLTexture() { return mTexture; }
