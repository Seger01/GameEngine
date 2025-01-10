/**
 * @file Renderer.cpp
 * @brief Implementation of the Renderer class, responsible for rendering textures, shapes, and text.
 */

#include <iostream>

#include <SDL_render.h>
#include <SDL_ttf.h>

#include "Animation.h"
#include "FSConverter.h"
#include "Point.h"
#include "Rect.h"
#include "Renderer.h"
#include "Window.h"

/**
 * @brief Constructor for the Renderer class. Initializes the renderer and font.
 * @param window The window to render to.
 */
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

/**
 * @brief Destructor for the Renderer class. Frees the renderer and font.
 */
Renderer::~Renderer()
{
	SDL_DestroyRenderer(this->mRenderer);

	// Close the font
	TTF_CloseFont(mFont);

	// Quit SDL_ttf
	TTF_Quit();
}

/**
 * @brief Renders a texture to the screen.
 * @param aTexture The texture to render.
 * @param aSourceRect The source rectangle of the texture.
 * @param aLocation The location to render the texture.
 * @param aWidth The width of the texture.
 * @param aHeight The height of the texture.
 * @param aFlipX Whether to flip the texture horizontally.
 * @param aFlipY Whether to flip the texture vertically.
 * @param aRotation The rotation of the texture.
 * @param aColor The color of the texture.
 */

void Renderer::renderTexture(const Texture& aTexture, const Rect& aSourceRect, const Vector2& aLocation,
							 const int aWidth, const int aHeight, const bool aFlipX, const bool aFlipY,
							 const float aRotation, const Color& aColor, const Point& aRotationalCenter) const
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

	SDL_Point center = {aRotationalCenter.x, aRotationalCenter.y}; // Rotation center

	// Render the texture with flipping and rotation
	SDL_RenderCopyEx(mRenderer,		   // The renderer associated with the texture
					 sdlTexture,	   // The texture to render
					 sourceRect.get(), // The source rectangle (nullptr means the entire texture)
					 &dstRect,		   // The destination rectangle
					 aRotation,		   // The angle of rotation (in degrees)
					 &center,		   // The point around which to rotate (nullptr means center)
					 flip			   // The flipping mode
	);
}

/**
 * @brief Renders a square to the screen.
 * @param aLocation The location of the square.
 * @param aWidth The width of the square.
 * @param aHeight The height of the square.
 * @param rotation The rotation of the square.
 * @param aColor The color of the square.
 * @param aFill Whether to fill the square.
 */
void Renderer::renderSquare(const Vector2& aLocation, const int aWidth, const int aHeight, const float rotation,
							const Color& aColor, const bool aFill, const Point& aRotationalCenter) const
{
	// Create a rectangle to define the size and position
	SDL_Rect rect;
	rect.x = static_cast<int>(aLocation.x);
	rect.y = static_cast<int>(aLocation.y);
	rect.w = aWidth;
	rect.h = aHeight;

	if (rotation == 0.0f)
	{
		if (aColor.a != 255)
			SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

		// Set the render color
		SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a);
		// If no rotation, use the SDL drawing functions directly
		if (aFill)
		{
			SDL_RenderFillRect(mRenderer, &rect);
		}
		else
		{
			SDL_RenderDrawRect(mRenderer, &rect);
		}
	}
	else
	{
		// Set the render color
		SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a);

		// Create a temporary texture with transparency
		SDL_Texture* texture =
			SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, aWidth, aHeight);

		// Enable blending on the texture
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		// Set the texture as the rendering target
		SDL_SetRenderTarget(mRenderer, texture);

		// Clear the texture with full transparency
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0); // RGBA(0, 0, 0, 0) = transparent
		SDL_RenderClear(mRenderer);

		// Draw the rectangle outline or fill on the texture
		SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a);
		if (aFill)
		{
			SDL_Rect fillRect = {0, 0, aWidth, aHeight};
			SDL_RenderFillRect(mRenderer, &fillRect);
		}
		else
		{
			SDL_Rect outlineRect = {0, 0, aWidth, aHeight};
			SDL_RenderDrawRect(mRenderer, &outlineRect);
		}

		// Reset the rendering target to the default renderer
		SDL_SetRenderTarget(mRenderer, nullptr);

		// Render the texture with rotation
		SDL_Point center = {aRotationalCenter.x, aRotationalCenter.y}; // Rotation center
		SDL_RenderCopyEx(mRenderer, texture, nullptr, &rect, rotation, &center, SDL_FLIP_NONE);

		// Destroy the temporary texture
		SDL_DestroyTexture(texture);
	}
}

/**
 * @brief Draws a circle to the screen.
 * @param center The center of the circle.
 * @param radius The radius of the circle.
 * @param aColor The color of the circle.
 * @param aFill Whether to fill the circle.
 */
void Renderer::drawCircle(const Vector2& center, const int radius, const Color& aColor, const bool aFill) const
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

/**
 * @brief Renders text to the screen.
 * @param aText The text to render.
 * @param aLocation The location to render the text.
 * @param aColor The color of the text.
 * @param scaleX The horizontal scale of the text.
 * @param scaleY The vertical scale of the text.
 */
void Renderer::renderText(const std::string& aText, const Vector2& aLocation, const Color& aColor, const float scaleX,
						  const float scaleY) const
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

/**
 * @brief Calculates the size of a text string.
 * @param font The font to use.
 * @param text The text to measure.
 * @param width The width of the text.
 * @param height The height of the text.
 * @return True if the text size was successfully calculated, false otherwise.
 */
bool Renderer::calculateTextSize(const std::string& font, const std::string& text, int& width, int& height) const
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

/**
 * @brief Sets the viewport of the renderer.
 * @param viewport The viewport to set.
 */
void Renderer::setViewport(const Rect& viewport) const
{
	SDL_Rect rect = ((SDL_Rect)viewport);
	SDL_RenderSetViewport(mRenderer, &rect);
}

/**
 * @brief Clears the renderer with a specified color.
 * @param aColor The color to clear the renderer with.
 */
void Renderer::clear(const Color& aColor) const
{
	SDL_SetRenderDrawColor(mRenderer, aColor.r, aColor.g, aColor.b, aColor.a); // Red
	SDL_RenderClear(mRenderer);
}

/**
 * @brief Presents the renderer to the screen.
 */
void Renderer::show() const { SDL_RenderPresent(mRenderer); }

/**
 * @brief Retrieves the SDL renderer.
 * @return The SDL renderer.
 */
SDL_Renderer*& Renderer::getSDLRenderer() { return mRenderer; }
