/**
 * @file Sprite.h
 * @brief Sprite class header file.
 */

#pragma once

#include <string>

#include "Color.h"
#include "Component.h"
#include "Rect.h"
#include "Texture.h"
#include "Transform.h"

/**
 * @class Sprite
 * @brief Sprite class, used to render textures.
 */
class Sprite : public Component
{
public:
	Sprite(const Texture& aTexture, int aWidth, int aHeight, const Rect& aSourceRect = Rect(),
		   const std::string& aTag = "defaultSprite");
	~Sprite();

	// Rule of Five
	Sprite(const Sprite& other);				// Copy constructor
	Sprite& operator=(const Sprite& other);		// Copy assignment operator
	Sprite(Sprite&& other) noexcept;			// Move constructor
	Sprite& operator=(Sprite&& other) noexcept; // Move assignment operator

	// Override the clone method
	std::unique_ptr<Component> clone() const override;

	const Texture& getTexture() const;

	Transform getRelativePosition() const;
	void setRelativePosition(const Transform& aNewTransform);

	const Rect& getSource() const;
	void setSource(const Rect& aSourceRect);

	Color getColorFilter() const;
	void setColorFilter(Color aColor);

	void setFlipX(bool state) { mFlipX = state; }

	bool getFlipX() const { return mFlipX; }

	void setFlipY(bool state) { mFlipY = state; }

	bool getFlipY() const { return mFlipY; }

	void setWidth(int aWidth) { mWidth = aWidth; }

	int getWidth() const { return mWidth; }

	void setHeight(int aHeight) { mHeight = aHeight; }

	int getHeight() const { return mHeight; }

	void setLayer(int aLayer) { mLayer = aLayer; }

	int getLayer() { return mLayer; }

private:
	Transform mTransform; // Relative position to the parent GameObject

	const Texture& mTexture; // Pointer to the texture
	Rect mSourceRect;		 // Source rectangle of the texture
	Color mColorFilter;		 // Color filter of the sprite

	float mWidth = 0;  // Width of the sprite
	float mHeight = 0; // Height of the sprite
	bool mFlipX;	   // Whether the sprite should flip horizontally
	bool mFlipY;	   // Whether the sprite should flip vertically

	int mLayer = 0; // Layer of the sprite
};
