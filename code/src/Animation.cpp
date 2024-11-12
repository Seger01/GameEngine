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
