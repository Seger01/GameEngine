/**
 * @file Renderer.h
 * @brief Header file for the Renderer class, responsible for rendering textures, shapes, and text.
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Color.h"
#include "Point.h"
#include "Rect.h"
#include "Texture.h"
#include "Transform.h"
#include "Window.h"

class Animation;

/**
 * @class Renderer
 * @brief Handles rendering of textures, shapes, and text.
 *
 * This class manages the rendering of textures, shapes, and text on the screen.
 */
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
					   const Color& aColor, const Point& aRotationalCenter = {0, 0}) const;

	void renderSquare(const Vector2& aLocation, const int aWidth, const int aHeight, const float rotation,
					  const Color& aColor, const bool aFill, const Point& aRotationalCenter = {0, 0}) const;

	void drawCircle(const Vector2& center, const int radius, const Color& aColor, const bool aFill) const;

	void renderText(const std::string& aText, const Vector2& aLocation, const Color& aColor, const float scaleX,
					const float scaleY) const;

	bool calculateTextSize(const std::string& font, const std::string& text, int& width, int& height) const;

	SDL_Renderer*& getSDLRenderer();

private:
	TTF_Font* mFont = nullptr;

	/// @brief The SDL renderer.
	SDL_Renderer* mRenderer = nullptr;
};

#endif
