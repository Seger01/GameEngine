#include "Renderer.h"
#include "Point.h"
#include <iostream>

#include "Animation.h"
#include "SDL_render.h"
#include "Window.h"

Renderer::Renderer(Window& window) {
    // Create renderer
    mRenderer = SDL_CreateRenderer(window.getSDLWindow(), -1, SDL_RENDERER_ACCELERATED);
    if (mRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
}

void Renderer::renderTexture(Texture& aTexture, Point& location, bool aFlipX, bool aFlipY, float aRotation) {}

Renderer::~Renderer() { SDL_DestroyRenderer(this->mRenderer); }

void Renderer::clear() { SDL_RenderClear(mRenderer); }

void Renderer::show() { SDL_RenderPresent(mRenderer); }

SDL_Renderer*& Renderer::getSDLRenderer() { return mRenderer; }
