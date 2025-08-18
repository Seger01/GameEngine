#include "Rendering/Texture.h"
#include "Rendering/Renderer.h"
#include <stdexcept>

// Texture::Texture(SDL_Renderer*& aRenderer) { this->mRenderer = aRenderer; }
Texture::Texture(SML::Texture* aTexture, int aID) : mTexture(aTexture), mID(aID)
{
	if (mTexture == nullptr)
	{
		throw std::runtime_error("Failed to create texture. SDL_Texture is null.");
	}
}

SML::Texture* Texture::getSMLTexture() const { return mTexture; }

Texture::~Texture() {}
