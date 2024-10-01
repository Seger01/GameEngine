#include "SpriteAtlas.h"

// SpriteAtlas::SpriteAtlas(SDL_Renderer*& renderer) : renderer{renderer} { textureIsLoaded = false; }

// SpriteAtlas::SpriteAtlas(SDL_Renderer*& renderer, std::string filePath) : renderer{renderer} {
//     this->loadTexture(filePath);
// }

SpriteAtlas::SpriteAtlas(Renderer*& aRenderer, std::string aFilePath) : mRenderer{aRenderer} {
    this->loadTexture(aFilePath);
}

SpriteAtlas::~SpriteAtlas() {
    this->unloadTexture();
    std::cout << "~SpriteAtlas()" << std::endl;
}

void SpriteAtlas::loadTexture(std::string aFilePath) {
    // // Load image
    // SDL_Surface* tempSurface = IMG_Load(aFilePath.c_str());
    // if (tempSurface == NULL) {
    //     printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    //     return;
    // }
    //
    // loadedTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    // SDL_FreeSurface(tempSurface);

    mTexture = new Texture(mRenderer);
    mTexture->load(aFilePath);

    textureIsLoaded = true;

    return;
}

void SpriteAtlas::unloadTexture() { SDL_DestroyTexture(mTexture->getSDL_Texture()); }

Animation& SpriteAtlas::getAnimation(Rectangle aStartingFrame, int aAmountOfFrames) {
    std::cout << "getAnimation() " << mCreatedAnimations.size() << std::endl;

    mCreatedAnimations.emplace_back(mTexture, aStartingFrame, aAmountOfFrames);

    return mCreatedAnimations[mCreatedAnimations.size() - 1];
}
