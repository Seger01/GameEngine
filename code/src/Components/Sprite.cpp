#include "Components/Sprite.h"

Sprite::Sprite(Texture* aTexture, int aWidth, int aHeight, Rect aSourceRect, std::string aTag)
	: Component{aTag}, mTexture(aTexture), mWidth(aWidth), mHeight(aHeight), mSourceRect(aSourceRect),
	  mTransform(Transform()), mFlipX(false), mFlipY(false), mLayer(0), mColorFilter(Color(255, 255, 255, 255))
{
}

// Copy constructor
Sprite::Sprite(const Sprite& other)
	: Component(other),			// Copy base class members
	  mTexture(other.mTexture), // Shallow copy texture pointer
	  mTransform(other.mTransform), mSourceRect(other.mSourceRect), mSprite(other.mSprite), mWidth(other.mWidth),
	  mHeight(other.mHeight), mFlipX(other.mFlipX), mFlipY(other.mFlipY), mLayer(other.mLayer),
	  mColorFilter(other.mColorFilter)
{
}

// Copy assignment operator
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

// Move constructor
Sprite::Sprite(Sprite&& other) noexcept
	: Component(std::move(other)), // Move base class members
	  mTexture(other.mTexture),	   // Transfer texture pointer
	  mTransform(std::move(other.mTransform)), mSourceRect(std::move(other.mSourceRect)),
	  mSprite(std::move(other.mSprite)), mWidth(other.mWidth), mHeight(other.mHeight), mFlipX(other.mFlipX),
	  mFlipY(other.mFlipY), mLayer(other.mLayer)
{
	other.mTexture = nullptr; // Nullify moved-from texture pointer
}

// Move assignment operator
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

Sprite::~Sprite()
{
	// Cleanup resources if necessary
}

std::unique_ptr<Component> Sprite::clone() const { return std::make_unique<Sprite>(*this); }

Texture* Sprite::getTexture() { return mTexture; }

Transform Sprite::getRelativePosition() { return mTransform; }

void Sprite::setRelativePosition(Transform aNewTransform) { mTransform = aNewTransform; }

Rect Sprite::getSource() { return mSourceRect; }

void Sprite::setSource(Rect aSourceRect) { mSourceRect = aSourceRect; }

Color Sprite::getColorFilter() { return mColorFilter; }

void Sprite::setColorFilter(Color aColor) { mColorFilter = aColor; }
