/**
 * @file Sprite.h
 * @brief Sprite class header file.
 */

#pragma once

#include <string>

#include "Component.h"
#include "Global/Color.h"
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

	void setWidth(float aWidth) { mWidth = aWidth; }

	float getWidth() const { return mWidth; }

	void setHeight(float aHeight) { mHeight = aHeight; }

	float getHeight() const { return mHeight; }

	void setLayer(int aLayer) { mLayer = aLayer; }

	int getLayer() { return mLayer; }

private:
	/// @brief Transform of the sprite.
	Transform mTransform;

	/// @brief Texture of the sprite.
	const Texture& mTexture;
	/// @brief Source rectangle of the texture.
	Rect mSourceRect;
	/// @brief Color filter of the sprite.
	Color mColorFilter;

	/// @brief Width of the sprite.
	float mWidth = 0;
	/// @brief Height of the sprite.
	float mHeight = 0;
	/// @brief Whether the sprite should flip horizontally.
	bool mFlipX;
	/// @brief Whether the sprite should flip vertically.
	bool mFlipY;

	/// @brief Layer the sprite should be rendered on.
	int mLayer = 0;
};
