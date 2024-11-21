#include "Animation.h"
#include "GameObject.h"
#include "Time.h"

Animation::Animation(std::vector<Sprite*> aAnimationFrames, int aTimeBetweenFrames, bool aIsLooping)
    : mTimeBetweenFrames(aTimeBetweenFrames), mIsLooping(aIsLooping), mFlipX(false), mFlipY(false) {
    // Transfer each raw pointer to a unique_ptr and store in mAnimationFrames
    for (Sprite* sprite : aAnimationFrames) {
        mAnimationFrames.push_back(std::unique_ptr<Sprite>(sprite));
    }
}

Animation::~Animation() = default;

// Copy constructor
Animation::Animation(const Animation& other)
    : Component(other), mTransform(other.mTransform), mFlipX(other.mFlipX), mFlipY(other.mFlipY),
      mTimeBetweenFrames(other.mTimeBetweenFrames), mIsLooping(other.mIsLooping), mLayer(other.mLayer) {
    // Deep copy of mAnimationFrames
    for (const auto& frame : other.mAnimationFrames) {
        if (frame) {
            mAnimationFrames.push_back(std::make_unique<Sprite>(*frame));
        }
    }
}

// Copy assignment operator
Animation& Animation::operator=(const Animation& other) {
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
    for (const auto& frame : other.mAnimationFrames) {
        if (frame) {
            mAnimationFrames.push_back(std::make_unique<Sprite>(*frame));
        }
    }

    return *this;
}

// Move constructor
Animation::Animation(Animation&& other) noexcept
    : Component(std::move(other)), mTransform(std::move(other.mTransform)), mFlipX(other.mFlipX), mFlipY(other.mFlipY),
      mAnimationFrames(std::move(other.mAnimationFrames)), // Transfer ownership of unique pointers
      mTimeBetweenFrames(other.mTimeBetweenFrames), mIsLooping(other.mIsLooping), mLayer(other.mLayer) {
    other.mFlipX = false;
    other.mFlipY = false;
    other.mTimeBetweenFrames = 0;
    other.mIsLooping = false;
    other.mLayer = 0;
}

// Move assignment operator
Animation& Animation::operator=(Animation&& other) noexcept {
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

Transform Animation::getTransform() {
    Transform parentTransform = this->mGameObject->getTransform();
    return parentTransform + mTransform;
}

void Animation::setTransform(Transform aNewTransform) { mTransform = aNewTransform; }

Sprite* Animation::getFrame(int aFrameIndex) {
    if (aFrameIndex >= mAnimationFrames.size()) {
        return nullptr;
    }

    Sprite* sprite = mAnimationFrames[aFrameIndex].get();
    sprite->setFlipX(mFlipX);
    sprite->setFlipY(mFlipY);

    return sprite;
}

Sprite* Animation::getCurrentFrame() {
    int frameIndex =
        (static_cast<int>(Time::ticks * 1000) % (mTimeBetweenFrames * mAnimationFrames.size())) / mTimeBetweenFrames;
    return getFrame(frameIndex);
}

Color Animation::getColorFilter() {
    for (auto& frame : mAnimationFrames) {
        if (frame) {
            return frame->getColorFilter();
        }
    }
    return Color(255, 255, 255, 255);
}
void Animation::setColorFilter(Color aColor) {
    for (auto& frame : mAnimationFrames) {
        if (frame) {
            frame->setColorFilter(aColor);
        }
    }
}

int Animation::getTimeBetweenFrames() { return mTimeBetweenFrames; }
void Animation::setTimeBetweenFrames(int aTimeBetweenFrames) { mTimeBetweenFrames = aTimeBetweenFrames; }

bool Animation::isLooping() { return mIsLooping; }
void Animation::setIsLooping(bool aIsLooping) { mIsLooping = aIsLooping; }

void Animation::setFlipX(bool state) { mFlipX = state; }
bool Animation::getFlipX() { return mFlipX; }

void Animation::setFlipY(bool state) { mFlipY = state; }
bool Animation::getFlipY() { return mFlipY; }

int Animation::getFrameCount() { return mAnimationFrames.size(); }

void Animation::setLayer(int aLayer) { mLayer = aLayer; }
int Animation::getLayer() { return mLayer; }
