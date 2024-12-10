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

class Renderer
{
public:
	Renderer(Window& window);
	~Renderer();

	void clear(Color aColor);
	void show();
	void setViewport(Rect& viewport);

	void renderTexture(Texture& aTexture, Rect aSourceRect, Vector2 aLocation, int aWidth, int aHeight, bool aFlipX,
					   bool aFlipY, float aRotation, Color aColor);

	void renderSquare(Vector2 aLocation, int aWidth, int aHeight, float rotation, Color aColor, bool aFill,
					  Point aRotationalCenter = Point{0, 0});

	void drawCircle(Vector2 center, int radius, Color aColor, bool aFill);

	void renderPolygon(const std::vector<Vector2>& vertices, const Color& color, bool filled);

	void renderText(const std::string& aText, Vector2 aLocation, Color aColor, float scaleX, float scaleY);

	bool calculateTextSize(const std::string& font, const std::string& text, int& width, int& height);

	SDL_Renderer*& getSDLRenderer();

private:
	TTF_Font* mFont = nullptr;

	SDL_Renderer* mRenderer = nullptr;
};

#endif
