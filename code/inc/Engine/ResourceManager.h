/**
 * @file ResourceManager.h
 * @brief ResourceManager class header file.
 */

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Components/Sprite.h"
#include "Renderer.h"
#include "SpriteDef.h"
#include "Texture.h"

/**
 * @class ResourceManager
 * @brief Class for managing resources such as textures and audio.
 */
class ResourceManager
{
public:
	friend class EngineBravo;
	void setRenderer(Renderer* aRenderer);

	Animation* loadAnimation(std::vector<SpriteDef> aSpriteDefs, int aTimeBetweenFrames, bool aIsLooping);
	Texture* loadTexture(const std::string& aPngPath);
	Sprite* createSprite(SpriteDef aSpriteDef);

	// Load audio function placeholder
	void loadAudio();

private:
	ResourceManager();
	ResourceManager(Renderer* aRenderer);
	~ResourceManager();

	Renderer* mRenderer;

	// Map for storing textures, keyed by file path
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextureMap;
};

#endif
