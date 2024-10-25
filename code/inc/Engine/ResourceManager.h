#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "Renderer.h"
#include "Texture.h"

class ResourceManager {
public:
    friend class EngineBravo;
    void setRenderer(Renderer* aRenderer);

    Texture* loadTexture(const std::string& aPngPath);

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
