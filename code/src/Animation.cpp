/**
 * @file Animation.cpp
 * @brief Animation class implementation file.
 */

#include "Animation.h"
#include "GameObject.h"
#include "Time.h"

/**
 * @brief Animation class constructor
 * @param aAnimationFrames A vector of Sprite references
 * @param aTimeBetweenFrames Time between each frame in milliseconds
 * @param aIsLooping Whether the animation should loop
 * @param aTag The tag of the animation
 */
Animation::Animation(std::vector<std::reference_wrapper<Sprite>> aAnimationFrames, int aTimeBetweenFrames,
					 bool aIsLooping, const std::string& aTag)
	: Component{aTag}, mTimeBetweenFrames(aTimeBetweenFrames), mIsLooping(aIsLooping), mFlipX(false), mFlipY(false),
	  mLayer(0)
{
	// Transfer each raw pointer to a unique_ptr and store in mAnimationFrames
	for (Sprite& sprite : aAnimationFrames)
	{
		mAnimationFrames.emplace_back(&sprite);
	}
}

/**
 * @brief Animation class destructor
 */
Animation::~Animation() = default;

/**
 * @brief Animation class copy constructor
 * @param other The Animation object to copy
 */
Animation::Animation(const Animation& other)
	: Component(other), mTransform(other.mTransform), mFlipX(other.mFlipX), mFlipY(other.mFlipY),
	  mTimeBetweenFrames(other.mTimeBetweenFrames), mIsLooping(other.mIsLooping), mLayer(other.mLayer)
{
	// Deep copy of mAnimationFrames
	for (const std::unique_ptr<Sprite>& frame : other.mAnimationFrames)
	{
		if (frame)
		{
			mAnimationFrames.push_back(std::make_unique<Sprite>(*frame));
		}
	}
}

/**
 * @brief Animation class copy assignment operator
 * @param other The Animation object to copy
 * @return The copied Animation object
 */
Animation& Animation::operator=(const Animation& other)
{
	if (this == &other)
		return *this;

	Component::operator=(other);
	mTransform = other.mTransform;
	mFlipX = other.mFlipX;
	mFlipY = other.mFlipY;
	mTimeBetweenFrames = other.mTimeBetweenFrames;
	mIsLooping = other.mIsLooping;
	mLayer = other.mLayer;

	// Deep copy of mAnimationFrames
	mAnimationFrames.clear();
	for (const std::unique_ptr<Sprite>& frame : other.mAnimationFrames)
	{
		if (frame)
		{
			mAnimationFrames.push_back(std::make_unique<Sprite>(*frame));
		}
	}

	return *this;
}

/**
 * @brief Animation class move constructor
 * @param other The Animation object to move
 */
Animation::Animation(Animation&& other) noexcept
	: Component(std::move(other)), mTransform(std::move(other.mTransform)), mFlipX(other.mFlipX), mFlipY(other.mFlipY),
	  mAnimationFrames(std::move(other.mAnimationFrames)), // Transfer ownership of unique pointers
	  mTimeBetweenFrames(other.mTimeBetweenFrames), mIsLooping(other.mIsLooping), mLayer(other.mLayer)
{
	other.mFlipX = false;
	other.mFlipY = false;
	other.mTimeBetweenFrames = 0;
	other.mIsLooping = false;
	other.mLayer = 0;
}

/**
 * @brief Animation class move assignment operator
 * @param other The Animation object to move
 * @return The moved Animation object
 */
Animation& Animation::operator=(Animation&& other) noexcept
{
	if (this == &other)
		return *this;

	Component::operator=(std::move(other));
	mTransform = std::move(other.mTransform);
	mFlipX = other.mFlipX;
	mFlipY = other.mFlipY;
	mAnimationFrames = std::move(other.mAnimationFrames); // Transfer ownership of unique pointers
	mTimeBetweenFrames = other.mTimeBetweenFrames;
	mIsLooping = other.mIsLooping;
	mLayer = other.mLayer;

	other.mFlipX = false;
	other.mFlipY = false;
	other.mTimeBetweenFrames = 0;
	other.mIsLooping = false;
	other.mLayer = 0;

	return *this;
}

/**
 * @brief Animation class clone method
 * @return A unique pointer to a new Animation object
 */
std::unique_ptr<Component> Animation::clone() const
{
	return std::make_unique<Animation>(*this); // Uses the copy constructor of Sprite
}

/**
 * @brief Get the transform of the animation
 * @return The transform of the animation
 */
Transform Animation::getTransform() const
{
	Transform parentTransform = this->mGameObject->getTransform();
	return parentTransform + mTransform;
}

/**
 * @brief Set the transform of the animation
 * @param aNewTransform The new transform of the animation
 */
void Animation::setTransform(const Transform& aNewTransform) { mTransform = aNewTransform; }

/**
 * @brief Get a frame of the animation
 * @param aFrameIndex The index of the frame to get
 * @return The frame at the specified index
 */
const Sprite& Animation::getFrame(int aFrameIndex) const
{
	if (aFrameIndex >= mAnimationFrames.size())
	{
		throw std::out_of_range("Frame index out of range");
	}

	Sprite& sprite = *mAnimationFrames[aFrameIndex].get();
	sprite.setFlipX(mFlipX);
	sprite.setFlipY(mFlipY);

	return sprite;
}

/**
 * @brief Get the current frame of the animation
 * @return The current frame of the animation
 */
const Sprite& Animation::getCurrentFrame() const
{
	int frameIndex =
		(static_cast<int>(Time::ticks * 1000) % (mTimeBetweenFrames * mAnimationFrames.size())) / mTimeBetweenFrames;
	return getFrame(frameIndex);
}

/**
 * @brief Get the color filter of the animation
 * @return The color filter of the animation
 */
Color Animation::getColorFilter() const
{
	for (const std::unique_ptr<Sprite>& frame : mAnimationFrames)
	{
		// Check if the frame is not nullptr
		if (frame)
		{
			return frame->getColorFilter();
		}
	}
	return Color(255, 255, 255, 255);
}

/**
 * @brief Set the color filter of the animation
 * @param aColor The new color filter of the animation
 */
void Animation::setColorFilter(const Color& aColor)
{
	for (std::unique_ptr<Sprite>& frame : mAnimationFrames)
	{
		// Check if the frame is not nullptr
		if (frame)
		{
			frame->setColorFilter(aColor);
		}
	}
	throw std::runtime_error("Cannot set color filter: no instantiated frames in the animation");
}

/**
 * @brief Get the time between frames of the animation
 * @return The time between frames of the animation
 */
int Animation::getTimeBetweenFrames() const { return mTimeBetweenFrames; }

/**
 * @brief Set the time between frames of the animation
 * @param aTimeBetweenFrames The new time between frames of the animation
 */
void Animation::setTimeBetweenFrames(int aTimeBetweenFrames) { mTimeBetweenFrames = aTimeBetweenFrames; }

/**
 * @brief Check if the animation is looping
 * @return True if the animation is looping, false otherwise
 */
bool Animation::isLooping() const { return mIsLooping; }

/**
 * @brief Set whether the animation is looping
 * @param aIsLooping Whether the animation should loop
 */
void Animation::setIsLooping(bool aIsLooping) { mIsLooping = aIsLooping; }

/**
 * @brief Set whether the animation should flip horizontally
 * @param state Whether the animation should flip horizontally
 */
void Animation::setFlipX(bool state) { mFlipX = state; }

/**
 * @brief Get whether the animation is flipped horizontally
 * @return True if the animation is flipped horizontally, false otherwise
 */
bool Animation::getFlipX() const { return mFlipX; }

/**
 * @brief Set whether the animation should flip vertically
 * @param state Whether the animation should flip vertically
 */
void Animation::setFlipY(bool state) { mFlipY = state; }

/**
 * @brief Get whether the animation is flipped vertically
 * @return True if the animation is flipped vertically, false otherwise
 */
bool Animation::getFlipY() const { return mFlipY; }

/**
 * @brief Get the number of frames in the animation
 * @return The number of frames in the animation
 */
int Animation::getFrameCount() const { return mAnimationFrames.size(); }

/**
 * @brief Set the layer of the animation
 * @param aLayer The new layer of the animation
 */
void Animation::setLayer(int aLayer) { mLayer = aLayer; }

int Animation::getLayer() const { return mLayer; }
