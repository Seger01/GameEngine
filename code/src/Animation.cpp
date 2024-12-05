#include "Animation.h"
#include "GameObject.h"
#include "Time.h"

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

Animation::~Animation() = default;

// Copy constructor
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

// Copy assignment operator
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

// Move constructor
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

// Move assignment operator
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

std::unique_ptr<Component> Animation::clone() const
{
	return std::make_unique<Animation>(*this); // Uses the copy constructor of Sprite
}

Transform Animation::getTransform() const
{
	Transform parentTransform = this->mGameObject->getTransform();
	return parentTransform + mTransform;
}

void Animation::setTransform(const Transform& aNewTransform) { mTransform = aNewTransform; }

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

const Sprite& Animation::getCurrentFrame() const
{
	int frameIndex =
		(static_cast<int>(Time::ticks * 1000) % (mTimeBetweenFrames * mAnimationFrames.size())) / mTimeBetweenFrames;
	return getFrame(frameIndex);
}

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

int Animation::getTimeBetweenFrames() const { return mTimeBetweenFrames; }

void Animation::setTimeBetweenFrames(int aTimeBetweenFrames) { mTimeBetweenFrames = aTimeBetweenFrames; }

bool Animation::isLooping() const { return mIsLooping; }

void Animation::setIsLooping(bool aIsLooping) { mIsLooping = aIsLooping; }

void Animation::setFlipX(bool state) { mFlipX = state; }

bool Animation::getFlipX() const { return mFlipX; }

void Animation::setFlipY(bool state) { mFlipY = state; }

bool Animation::getFlipY() const { return mFlipY; }

int Animation::getFrameCount() const { return mAnimationFrames.size(); }

void Animation::setLayer(int aLayer) { mLayer = aLayer; }

int Animation::getLayer() const { return mLayer; }
