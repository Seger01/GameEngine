#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

// #include "Texture.h"
// #include "Animation.h"
#include "Point.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Window.h"

class Animation;

class Renderer {
public:
    Renderer(Window& window);
    Renderer(SDL_Renderer*& aRenderer);
    ~Renderer();

    void clear();
    void show();

    // void renderTexture(Texture& aTexture, Point& location, bool aFlipX, bool aFlipY, float aRotation);
    // void drawRect(Point& aLocation, float aRotation);

    SDL_Renderer*& getSDLRenderer();

private:
    SDL_Renderer* mRenderer = nullptr;
};

#endif
