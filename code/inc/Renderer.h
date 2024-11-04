#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "Color.h"
#include "Rect.h"
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

    void renderTexture(Texture& aTexture, Rect aSourceRect, Vector2 aLocation, int aWidth, int aHeight, bool aFlipX,
                       bool aFlipY, float aRotation);

    void renderSquare(Vector2 aLocation, int aWidth, int aHeight, float rotation, Color aColor, bool aFill);
    void renderSquare(Vector2 aLocation, int aWidth, int aHeight, Color aColor, bool aFill);

    void renderText(const std::string& aText, Vector2 aLocation, Color aColor);

    SDL_Renderer*& getSDLRenderer();

private:
    TTF_Font* mFont = nullptr;

    SDL_Renderer* mRenderer = nullptr;
};

#endif
