/**
 * @file ResourceManager.cpp
 * @brief ResourceManager class implementation file.
 */

#include "ResourceManager.h"

#include <iostream>

#include "Animation.h"
#include "FSConverter.h"

/**
 * @brief ResourceManager class default constructor
 */
ResourceManager::ResourceManager() : mRenderer(nullptr) {}

/**
 * @brief ResourceManager class constructor
 * @param aRenderer The renderer to use
 */
ResourceManager::ResourceManager(Renderer* aRenderer) : mRenderer(aRenderer) {}

/**
 * @brief ResourceManager class destructor
 */
ResourceManager::~ResourceManager()
{
	for (auto& texture : mTextureMap)
	{
		SDL_DestroyTexture(texture.second->getSDLTexture());
	}
	mTextureMap.clear();
}

/**
 * @brief Set the renderer to use
 * @param aRenderer The renderer to use
 */
void ResourceManager::setRenderer(Renderer* aRenderer) { mRenderer = aRenderer; }

/**
 * @brief Load a texture from a PNG file
 * @param aPngPath The path to the PNG file
 * @return A pointer to the loaded texture
 */
Texture* ResourceManager::loadTexture(const std::string& aPngPath)
{
	if (mRenderer == nullptr)
	{
		std::cout << "Geen renderer ingeladen bij Resource manager" << std::endl;
		return nullptr;
	}
	// Check if the texture is already loaded
	auto it = mTextureMap.find(aPngPath);
	if (it != mTextureMap.end())
	{
		// Return the already loaded texture
		return it->second.get();
	}

	// Load the texture from the specified path
	SDL_Texture* sdlTexture =
		IMG_LoadTexture(mRenderer->getSDLRenderer(), FSConverter().getResourcePath(aPngPath).c_str());
	if (!sdlTexture)
	{
		std::cerr << "Failed to load texture from " << aPngPath << ": " << SDL_GetError() << std::endl;
		return nullptr;
	}

	static int textureIDCounter = 0;
	textureIDCounter++;
	// Create a new Texture object
	auto texture = std::make_unique<Texture>(sdlTexture, textureIDCounter);

	// Store the texture in the map and return its pointer
	Texture* texturePtr = texture.get();
	mTextureMap[aPngPath] = std::move(texture);
	return texturePtr;
}

/**
 * @brief Create a sprite from a sprite definition
 * @param aSpriteDef The sprite definition
 * @return A pointer to the created sprite
 */
Sprite* ResourceManager::createSprite(SpriteDef aSpriteDef)
{
	Sprite* createdSprite = nullptr;

	createdSprite =
		new Sprite(*loadTexture(aSpriteDef.texturePath), aSpriteDef.width, aSpriteDef.height, aSpriteDef.sourceRect);

	return createdSprite;
}

/**
 * @brief Load an animation from a vector of sprite definitions
 * @param aSpriteDefs The sprite definitions
 * @param aTimeBetweenFrames The time between each frame in milliseconds
 * @param aIsLooping Whether the animation should loop
 * @return A pointer to the loaded animation
 */
Animation* ResourceManager::loadAnimation(std::vector<SpriteDef> aSpriteDefs, int aTimeBetweenFrames, bool aIsLooping)
{
	std::vector<std::reference_wrapper<Sprite>> animationFrames;
	for (SpriteDef spriteDef : aSpriteDefs)
	{
		animationFrames.push_back(*createSprite(spriteDef));
	}

	return new Animation(animationFrames, aTimeBetweenFrames, aIsLooping);
}
