#include "Renderer.h"
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

Renderer::Renderer(SDL_Renderer*& aRenderer) : mRenderer(aRenderer) {}

Renderer::~Renderer() { SDL_DestroyRenderer(this->mRenderer); }

void Renderer::clear() { SDL_RenderClear(mRenderer); }
// void Renderer::renderCopy(Texture*& aTexture) {
//
//
//
//     // SDL_RenderCopy(mRenderer, , &srcRect, &destRect);
// }

void Renderer::renderAnimation(Animation& animation, Rectangle aDestination) {
    SDL_Texture* texture = animation.getTexture()->getSDL_Texture();
    Rectangle srcRect = animation.getCurrentFrame();

    SDL_Rect sdlsrcRect = (SDL_Rect)srcRect;
    SDL_Rect sdldestRect = (SDL_Rect)aDestination;

    SDL_RenderCopy(mRenderer, texture, &sdlsrcRect, &sdldestRect);
}

void Renderer::present() { SDL_RenderPresent(mRenderer); }

SDL_Renderer*& Renderer::getSDLRenderer() { return mRenderer; }
