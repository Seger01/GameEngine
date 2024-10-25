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

void Renderer::renderTexture(Texture& aTexture, Vector2 aLocation, int aWidth, int aHeight, bool aFlipX, bool aFlipY,
                             float aRotation) {
    std::cout << "renderTexture called" << std::endl;
    // // Get the SDL_Texture from the Texture class
    // SDL_Texture* sdlTexture = aTexture.getSDLTexture();
    //
    // // Define the destination rectangle where the texture will be drawn
    // SDL_Rect dstRect;
    // dstRect.x = aLocation.x;
    // dstRect.y = aLocation.y;
    // dstRect.w = aWidth;
    // dstRect.h = aHeight;
    //
    // // Set the flipping mode based on input flags
    // SDL_RendererFlip flip = SDL_FLIP_NONE;
    // if (aFlipX && aFlipY) {
    //     flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    // } else if (aFlipX) {
    //     flip = SDL_FLIP_HORIZONTAL;
    // } else if (aFlipY) {
    //     flip = SDL_FLIP_VERTICAL;
    // }
    //
    // // Render the texture with flipping and rotation
    // SDL_RenderCopyEx(mRenderer,  // The renderer associated with the texture
    //                  sdlTexture, // The texture to render
    //                  nullptr,    // The source rectangle (nullptr means the entire texture)
    //                  &dstRect,   // The destination rectangle
    //                  aRotation,  // The angle of rotation (in degrees)
    //                  nullptr,    // The point around which to rotate (nullptr means center)
    //                  flip        // The flipping mode
    // );
    // Query the texture to get its width and height
    int width, height;
    width = aWidth;
    height = aHeight;

    // Set the destination rectangle for rendering the player texture
    SDL_Rect dstRect = {(int)aLocation.x, (int)aLocation.y, width, height};

    // Render the texture to the screen
    SDL_RenderCopy(mRenderer, aTexture.getSDLTexture(), nullptr, &dstRect);
}

void Renderer::renderSquare(Vector2 aLocation, int aWidth, int aHeight, Color aColor, bool aFill) {
    std::cout << "RenderSquare called()" << std::endl;
    // Set the draw color for the renderer (red, green, blue, alpha)
    SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a); // Red

    // Create a rectangle at position (200, 150) with width 400 and height 300
    SDL_Rect rect = {static_cast<int>(aLocation.x), static_cast<int>(aLocation.y), aWidth, aHeight};

    if (aFill) {
        // Set a new color (green) and fill the rectangle
        SDL_RenderFillRect(mRenderer, &rect);
    } else {
        // Draw an empty rectangle (outline)
        SDL_RenderDrawRect(mRenderer, &rect);
    }
}

Renderer::~Renderer() { SDL_DestroyRenderer(this->mRenderer); }

void Renderer::clear(Color aColor) {
    SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a); // Red
    SDL_RenderClear(mRenderer);
}

void Renderer::show() { SDL_RenderPresent(mRenderer); }

SDL_Renderer*& Renderer::getSDLRenderer() { return mRenderer; }
