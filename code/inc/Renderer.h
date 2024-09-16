#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

// #include "Texture.h"
// #include "Animation.h"
#include "Rectangle.h"
#include "Window.h"

class Animation;

class Renderer {
public:
    Renderer(Window& window);
    Renderer(SDL_Renderer*& aRenderer);
    ~Renderer();

    void clear();
    void renderCopy();
    void present();
    void renderAnimation(Animation& animation, Rectangle aDestination);

    SDL_Renderer*& getSDLRenderer();

private:
    SDL_Renderer* mRenderer = nullptr;
};

#endif
