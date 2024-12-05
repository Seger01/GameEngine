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

class Renderer
{
public:
	Renderer(Window& window);
	~Renderer();

	void clear(const Color& aColor) const;
	void show() const;
	void setViewport(const Rect& viewport) const;

	void renderTexture(const Texture& aTexture, const Rect& aSourceRect, const Vector2& aLocation, const int aWidth,
					   const int aHeight, const bool aFlipX, const bool aFlipY, const float aRotation,
					   const Color& aColor) const;

	void renderSquare(const Vector2& aLocation, const int aWidth, const int aHeight, const float rotation,
					  const Color& aColor, const bool aFill) const;

	void drawCircle(const Vector2& center, const int radius, const Color& aColor, const bool aFill) const;

	void renderText(const std::string& aText, const Vector2& aLocation, const Color& aColor, const float scaleX,
					const float scaleY) const;

	bool calculateTextSize(const std::string& font, const std::string& text, int& width, int& height) const;

	SDL_Renderer*& getSDLRenderer();

private:
	TTF_Font* mFont = nullptr;

	SDL_Renderer* mRenderer = nullptr;
};

#endif
