#include "Components/Sprite.h"

/**
 * @brief Sprite class constructor
 * @param aTexture The texture of the sprite
 * @param aWidth The width of the sprite
 * @param aHeight The height of the sprite
 * @param aSourceRect The source rectangle of the sprite
 * @param aTag The tag of the sprite
 */

Sprite::Sprite(const Texture& aTexture, int aWidth, int aHeight, const Rect& aSourceRect, const std::string& aTag)
	: Component(aTag), mTexture(aTexture), mWidth(aWidth), mHeight(aHeight), mSourceRect(aSourceRect), mLayer(0),
	  mColorFilter(Color(255, 255, 255, 255)), mFlipX(false), mFlipY(false), mTransform(Transform())
{
}

/**
 * @brief Sprite class copy constructor
 * @param other The Sprite object to copy
 */

Sprite::Sprite(const Sprite& other)
	: Component(other),			// Copy base class members
	  mTexture(other.mTexture), // Copy the texture reference
	  mTransform(other.mTransform), mSourceRect(other.mSourceRect), mWidth(other.mWidth), mHeight(other.mHeight),
	  mFlipX(other.mFlipX), mFlipY(other.mFlipY), mLayer(other.mLayer), mColorFilter(other.mColorFilter)
{
}

/**
 * @brief Sprite class move constructor
 * @param other The Sprite object to move
 */
Sprite::Sprite(Sprite&& other) noexcept
	: Component(std::move(other)), // Move base class members
	  mTexture(other.mTexture),	   // Move texture reference (no need to nullify)
	  mTransform(std::move(other.mTransform)), mSourceRect(std::move(other.mSourceRect)), mWidth(other.mWidth),
	  mHeight(other.mHeight), mFlipX(other.mFlipX), mFlipY(other.mFlipY), mLayer(other.mLayer)
{
	// No need to nullify mTexture as it's a reference
}

/**
 * @brief Sprite class move assignment operator
 * @param other The Sprite object to move
 * @return The moved Sprite object
 */
Sprite& Sprite::operator=(Sprite&& other) noexcept
{
	if (this == &other)
		return *this;

	// Move base class members
	Component::operator=(std::move(other));

	// Do not move the texture reference, it will stay as is
	mTransform = std::move(other.mTransform);
	mSourceRect = std::move(other.mSourceRect);
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mFlipX = other.mFlipX;
	mFlipY = other.mFlipY;
	mLayer = other.mLayer;

	// Return the current object
	return *this;
}

/**
 * @brief Sprite class destructor
 */
Sprite::~Sprite()
{
	// Cleanup resources if necessary
}

/**
 * @brief Sprite class clone method
 * @return A unique pointer to a new Sprite object
 */
std::unique_ptr<Component> Sprite::clone() const { return std::make_unique<Sprite>(*this); }

/**
 * @brief Get the texture of the sprite
 * @return The texture of the sprite
 */
const Texture& Sprite::getTexture() const { return mTexture; }

/**
 * @brief Get the relative position of the sprite
 * @return The relative position of the sprite
 */
Transform Sprite::getRelativePosition() const { return mTransform; }

/**
 * @brief Set the relative position of the sprite
 * @param aNewTransform The new relative position of the sprite
 */
void Sprite::setRelativePosition(const Transform& aNewTransform) { mTransform = aNewTransform; }

/**
 * @brief Get the source rectangle of the sprite
 * @return The source rectangle of the sprite
 */
const Rect& Sprite::getSource() const { return mSourceRect; }

/**
 * @brief Set the source rectangle of the sprite
 * @param aSourceRect The new source rectangle of the sprite
 */
void Sprite::setSource(const Rect& aSourceRect) { mSourceRect = aSourceRect; }

/**
 * @brief Get the color filter of the sprite
 * @return The color filter of the sprite
 */
Color Sprite::getColorFilter() const { return mColorFilter; }

/**
 * @brief Set the color filter of the sprite
 * @param aColor The new color filter of the sprite
 */
void Sprite::setColorFilter(Color aColor) { mColorFilter = aColor; }
