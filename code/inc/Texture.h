/**
 * @file Texture.h
 * @brief Header file for the Texture class, responsible for managing SDL textures.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <SDL.h>
#include <SDL_image.h>

/**
 * @class Texture
 * @brief Manages SDL texture.
 */

class Texture
{
public:
	// Texture(SDL_Renderer*& aRenderer);
	Texture(SDL_Texture* aTexture, int aID = 0);
	~Texture();

	SDL_Texture* getSDLTexture() const;

	int getID() const { return mID; }

private:
	int mID = 0;
	SDL_Texture* mTexture = nullptr;

	bool mTextureLoaded = false;
};

#endif
