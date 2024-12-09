#include "Components/Sprite.h"

/**
 * @brief Sprite class constructor
 * @param aTexture The texture of the sprite
 * @param aWidth The width of the sprite
 * @param aHeight The height of the sprite
 * @param aSourceRect The source rectangle of the sprite
 * @param aTag The tag of the sprite
 */

Sprite::Sprite(Texture* aTexture, int aWidth, int aHeight, Rect aSourceRect, std::string aTag)
	: Component{aTag}, mTexture(aTexture), mWidth(aWidth), mHeight(aHeight), mSourceRect(aSourceRect),
	  mTransform(Transform()), mFlipX(false), mFlipY(false), mLayer(0), mColorFilter(Color(255, 255, 255, 255))
{
}

/**
 * @brief Sprite class copy constructor
 * @param other The Sprite object to copy
 */
Sprite::Sprite(const Sprite& other)
	: Component(other),			// Copy base class members
	  mTexture(other.mTexture), // Shallow copy texture pointer
	  mTransform(other.mTransform), mSourceRect(other.mSourceRect), mSprite(other.mSprite), mWidth(other.mWidth),
	  mHeight(other.mHeight), mFlipX(other.mFlipX), mFlipY(other.mFlipY), mLayer(other.mLayer),
	  mColorFilter(other.mColorFilter)
{
}

/**
 * @brief Sprite class move constructor
 * @param other The Sprite object to move
 */
Sprite& Sprite::operator=(const Sprite& other)
{
	if (this == &other)
		return *this;

	Component::operator=(other); // Copy base class members
	mTexture = other.mTexture;	 // Shallow copy texture pointer
	mTransform = other.mTransform;
	mSourceRect = other.mSourceRect;
	mSprite = other.mSprite;
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mFlipX = other.mFlipX;
	mFlipY = other.mFlipY;
	mLayer = other.mLayer;

	return *this;
}

/**
 * @brief Sprite class move constructor
 * @param other The Sprite object to move
 */
Sprite::Sprite(Sprite&& other) noexcept
	: Component(std::move(other)), // Move base class members
	  mTexture(other.mTexture),	   // Transfer texture pointer
	  mTransform(std::move(other.mTransform)), mSourceRect(std::move(other.mSourceRect)),
	  mSprite(std::move(other.mSprite)), mWidth(other.mWidth), mHeight(other.mHeight), mFlipX(other.mFlipX),
	  mFlipY(other.mFlipY), mLayer(other.mLayer)
{
	other.mTexture = nullptr; // Nullify moved-from texture pointer
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

	Component::operator=(std::move(other)); // Move base class members
	mTexture = other.mTexture;				// Transfer texture pointer
	mTransform = std::move(other.mTransform);
	mSourceRect = std::move(other.mSourceRect);
	mSprite = std::move(other.mSprite);
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mFlipX = other.mFlipX;
	mFlipY = other.mFlipY;
	mLayer = other.mLayer;

	other.mTexture = nullptr; // Nullify moved-from texture pointer

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
Texture* Sprite::getTexture() const { return mTexture; }

/**
 * @brief Get the relative position of the sprite
 * @return The relative position of the sprite
 */
Transform Sprite::getRelativePosition() const { return mTransform; }

/**
 * @brief Set the relative position of the sprite
 * @param aNewTransform The new relative position of the sprite
 */
void Sprite::setRelativePosition(Transform aNewTransform) { mTransform = aNewTransform; }

/**
 * @brief Get the source rectangle of the sprite
 * @return The source rectangle of the sprite
 */
Rect Sprite::getSource() const { return mSourceRect; }

/**
 * @brief Set the source rectangle of the sprite
 * @param aSourceRect The new source rectangle of the sprite
 */
void Sprite::setSource(Rect aSourceRect) { mSourceRect = aSourceRect; }

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
