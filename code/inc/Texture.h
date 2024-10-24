#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <SDL.h>
#include <SDL_image.h>

class Texture {
public:
    // Texture(SDL_Renderer*& aRenderer);
    Texture(SDL_Texture* aTexture);
    ~Texture();

    SDL_Texture* getSDLTexture();

private:
    SDL_Texture* mTexture = nullptr;

    bool mTextureLoaded = false;
};

#endif
