#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

#include "Color.h"
#include "Texture.h"
#include "Transform.h"
#include "Window.h"

class Animation;

class Renderer {
public:
    Renderer(Window& window);
    ~Renderer();

    void clear(Color aColor);
    void show();

    void renderTexture(Texture& aTexture, Vector2 aLocation, int aWidth, int aHeight, bool aFlipX, bool aFlipY,
                       float aRotation);

    void renderSquare(Vector2 aLocation, int aWidth, int aHeight, Color aColor, bool aFill);

    SDL_Renderer*& getSDLRenderer();

private:
    SDL_Renderer* mRenderer = nullptr;
};

#endif
