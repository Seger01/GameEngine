#include <iostream>

#include <SDL_render.h>
#include <SDL_ttf.h>

#include "Animation.h"
#include "FSConverter.h"
#include "Rect.h"
#include "Renderer.h"
#include "Window.h"

Renderer::Renderer(Window& window)
{
	// Create renderer
	mRenderer = SDL_CreateRenderer(window.getSDLWindow(), -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	// if (SDL_RenderSetVSync(mRenderer, 1) != 0) {
	//     std::cerr << "Warning: V-Sync is not supported or failed to enable!" << std::endl;
	// }

	// Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		SDL_Log("Unable to initialize SDL_ttf: %s", TTF_GetError());
		SDL_Quit();
		return;
	}

	// Load a font
	TTF_Font* font = TTF_OpenFont(FSConverter().getResourcePath("font/joystixmonospace.otf").c_str(), 22); // Specify
																										   // the font
																										   // path and
																										   // size
	if (!font)
	{
		SDL_Log("Failed to load font: %s", TTF_GetError());
		TTF_Quit();
		SDL_Quit();
		return;
	}

	mFont = font;
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(this->mRenderer);

	// Close the font
	TTF_CloseFont(mFont);

	// Quit SDL_ttf
	TTF_Quit();
}

void Renderer::renderTexture(Texture& aTexture, Rect aSourceRect, Vector2 aLocation, int aWidth, int aHeight,
							 bool aFlipX, bool aFlipY, float aRotation, Color aColor)
{
	// Get the SDL_Texture from the Texture class
	SDL_Texture* sdlTexture = aTexture.getSDLTexture();

	// Set the alpha modulation for the texture
	SDL_SetTextureAlphaMod(sdlTexture, aColor.a);

	// Set the color modulation for the texture
	SDL_SetTextureColorMod(sdlTexture, aColor.r, aColor.g, aColor.b);

	// Define the destination rectangle where the texture will be drawn
	SDL_Rect dstRect;
	dstRect.x = aLocation.x;
	dstRect.y = aLocation.y;
	dstRect.w = aWidth;
	dstRect.h = aHeight;

	// Define the source rectangle if specified
	std::unique_ptr<SDL_Rect> sourceRect(nullptr);
	if (aSourceRect.w != 0)
	{
		sourceRect = std::make_unique<SDL_Rect>(aSourceRect);
	}

	// Set the flipping mode based on input flags
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (aFlipX && aFlipY)
	{
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	}
	else if (aFlipX)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if (aFlipY)
	{
		flip = SDL_FLIP_VERTICAL;
	}

	// Render the texture with flipping and rotation
	SDL_RenderCopyEx(mRenderer,		   // The renderer associated with the texture
					 sdlTexture,	   // The texture to render
					 sourceRect.get(), // The source rectangle (nullptr means the entire texture)
					 &dstRect,		   // The destination rectangle
					 aRotation,		   // The angle of rotation (in degrees)
					 nullptr,		   // The point around which to rotate (nullptr means center)
					 flip			   // The flipping mode
	);
}

void Renderer::renderSquare(Vector2 aLocation, int aWidth, int aHeight, float rotation, Color aColor, bool aFill)
{
	// Create a rectangle to define the size and position
	SDL_Rect rect;
	rect.x = aLocation.x;
	rect.y = aLocation.y;
	rect.w = aWidth;
	rect.h = aHeight;

	// Set the render color
	SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a);

	// Create a texture from a surface
	SDL_Surface* surface = SDL_CreateRGBSurface(0, aWidth, aHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (aFill)
	{
		SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, aColor.r, aColor.g, aColor.b, aColor.a));
	}
	else
	{
		SDL_Rect outlineRect = {0, 0, aWidth, aHeight};
		SDL_FillRect(surface, &outlineRect, SDL_MapRGBA(surface->format, aColor.r, aColor.g, aColor.b, aColor.a));
	}

	// Create a texture from the surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
	SDL_FreeSurface(surface); // Free the surface after creating the texture

	// Render the texture with rotation
	SDL_Point center = {aWidth / 2, aHeight / 2}; // Rotation center
	SDL_RenderCopyEx(mRenderer, texture, nullptr, &rect, rotation, &center, SDL_FLIP_NONE);

	// Destroy the texture after rendering
	SDL_DestroyTexture(texture);
}

void Renderer::renderSquare(Vector2 aLocation, int aWidth, int aHeight, Color aColor, bool aFill)
{
	SDL_Rect rect;
	rect.x = aLocation.x;
	rect.y = aLocation.y;
	rect.w = aWidth;
	rect.h = aHeight;

	if (aColor.a != 255)
		SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a);
	if (aFill)
	{
		SDL_RenderFillRect(mRenderer, &rect);
	}
	else
	{
		SDL_RenderDrawRect(mRenderer, &rect);
	}
}

void Renderer::drawCircle(Vector2 center, int radius, Color aColor, bool aFill)
{
	// Set the render color
	SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a);

	if (aFill)
	{
		// Draw filled circle
		for (int w = 0; w < radius * 2; w++)
		{
			for (int h = 0; h < radius * 2; h++)
			{
				int dx = radius - w; // horizontal offset
				int dy = radius - h; // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius))
				{
					SDL_RenderDrawPoint(mRenderer, center.x + dx, center.y + dy);
				}
			}
		}
	}
	else
	{
		// Draw outline circle using the midpoint circle algorithm
		int x = radius;
		int y = 0;
		int decisionOver2 = 1 - x; // Decision criterion divided by 2

		while (x >= y)
		{
			// Draw the eight symmetrical points of the circle
			SDL_RenderDrawPoint(mRenderer, center.x + x, center.y + y);
			SDL_RenderDrawPoint(mRenderer, center.x + y, center.y + x);
			SDL_RenderDrawPoint(mRenderer, center.x - x, center.y + y);
			SDL_RenderDrawPoint(mRenderer, center.x - y, center.y + x);
			SDL_RenderDrawPoint(mRenderer, center.x - x, center.y - y);
			SDL_RenderDrawPoint(mRenderer, center.x - y, center.y - x);
			SDL_RenderDrawPoint(mRenderer, center.x + x, center.y - y);
			SDL_RenderDrawPoint(mRenderer, center.x + y, center.y - x);

			y++;
			if (decisionOver2 <= 0)
			{
				decisionOver2 += 2 * y + 1; // Change in decision criterion for y -> y+1
			}
			else
			{
				x--;
				decisionOver2 += 2 * (y - x) + 1; // Change for y -> y+1, x -> x-1
			}
		}
	}
}

void Renderer::renderText(const std::string& aText, Vector2 aLocation, Color aColor, float scaleX, float scaleY)
{
	// Determine if text is fully opaque
	bool isOpaque = (aColor.a == 255);

	// Create a surface from the text, choosing method based on transparency
	SDL_Surface* surface = nullptr;
	if (isOpaque)
	{
		surface = TTF_RenderText_Solid(mFont, aText.c_str(),
									   {static_cast<Uint8>(aColor.r), static_cast<Uint8>(aColor.g),
										static_cast<Uint8>(aColor.b), static_cast<Uint8>(aColor.a)});
	}
	else
	{
		surface = TTF_RenderText_Blended(mFont, aText.c_str(),
										 {static_cast<Uint8>(aColor.r), static_cast<Uint8>(aColor.g),
										  static_cast<Uint8>(aColor.b), static_cast<Uint8>(aColor.a)});
	}

	if (surface == nullptr)
	{
		std::cerr << "Failed to create surface from text: " << TTF_GetError() << std::endl;
		return;
	}

	// Create a texture from the surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
	if (texture == nullptr)
	{
		std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return;
	}

	// Set the texture alpha mode for translucency if needed
	if (!isOpaque)
	{
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(texture, aColor.a); // Apply the alpha level from the color
	}

	// Get the original width and height of the text
	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	// Create a rectangle for the position and scaled size
	SDL_Rect rect;
	rect.x = aLocation.x;
	rect.y = aLocation.y;
	rect.w = static_cast<int>(width * scaleX);
	rect.h = static_cast<int>(height * scaleY);

	// Render the texture
	SDL_RenderCopy(mRenderer, texture, nullptr, &rect);

	// Destroy the texture and surface after rendering
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

bool Renderer::calculateTextSize(const std::string& font, const std::string& text, int& width, int& height)
{
	if (TTF_SizeText(mFont, text.c_str(), &width, &height) == 0)
	{
		return true;
	}
	else
	{
		std::cerr << "Failed to calculate text size: " << TTF_GetError() << std::endl;
		return false;
	}
}

void Renderer::setViewport(const SDL_Rect& viewport) { SDL_RenderSetViewport(mRenderer, &viewport); }

void Renderer::clear(Color aColor)
{
	SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a); // Red
	SDL_RenderClear(mRenderer);
}

void Renderer::show() { SDL_RenderPresent(mRenderer); }

SDL_Renderer*& Renderer::getSDLRenderer() { return mRenderer; }
