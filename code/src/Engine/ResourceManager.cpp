#include "ResourceManager.h"

#include <iostream>

#include "Animation.h"
#include "FSConverter.h"

ResourceManager::ResourceManager() : mRenderer(nullptr) {}
ResourceManager::ResourceManager(Renderer* aRenderer) : mRenderer(aRenderer) {}

ResourceManager::~ResourceManager() {
    for (auto& texture : mTextureMap) {
        SDL_DestroyTexture(texture.second->getSDLTexture());
    }
    mTextureMap.clear();
}

void ResourceManager::setRenderer(Renderer* aRenderer) { mRenderer = aRenderer; }

Texture* ResourceManager::loadTexture(const std::string& aPngPath) {
    if (mRenderer == nullptr) {
        std::cout << "Geen renderer ingeladen bij Resource manager" << std::endl;
        return nullptr;
    }
    // Check if the texture is already loaded
    auto it = mTextureMap.find(aPngPath);
    if (it != mTextureMap.end()) {
        // Return the already loaded texture
        return it->second.get();
    }

    // Load the texture from the specified path
    // SDL_Texture* loadedTexture = IMG_LoadTexture(engine.getRenderSystem().getRenderer().getSDLRenderer(), );
    SDL_Texture* sdlTexture =
        IMG_LoadTexture(mRenderer->getSDLRenderer(), FSConverter().getResourcePath(aPngPath).c_str());
    if (!sdlTexture) {
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

Sprite* ResourceManager::createSprite(SpriteDef aSpriteDef) {
    Sprite* createdSprite = nullptr;

    createdSprite =
        new Sprite(loadTexture(aSpriteDef.texturePath), aSpriteDef.width, aSpriteDef.height, aSpriteDef.sourceRect);

    return createdSprite;
}

Animation* ResourceManager::loadAnimation(std::vector<SpriteDef> aSpriteDefs, int aTimeBetweenFrames, bool aIsLooping) {
    std::vector<Sprite*> animationFrames;
    for (SpriteDef spriteDef : aSpriteDefs) {
        animationFrames.push_back(createSprite(spriteDef));
    }

    return new Animation(animationFrames, aTimeBetweenFrames, aIsLooping);
}

void ResourceManager::loadAudio() {}
