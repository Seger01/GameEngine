/**
 * @file Texture.h
 * @brief Header file for the Texture class, responsible for managing SDL textures.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <SML/Texture.h>

/**
 * @class Texture
 * @brief Manages SDL texture.
 */

class Texture
{
public:
	Texture(SML::Texture* aTexture, int aID = 0);
	~Texture();

	SML::Texture* getSMLTexture() const;

	int getID() const { return mID; }

private:
	/// @brief The texture id
	int mID = 0;
	/// @brief The SDL texture
	SML::Texture* mTexture = nullptr;

	/// @brief Whether the texture is loaded
	bool mTextureLoaded = false;
};

#endif
