#include "Button.h"

Button::Button() : UIObject(), mWidth(0), mHeight(0), mInteractable(true), mOnClickCallback(nullptr), mHovered(false) {}

Button::~Button() {}

void Button::setOnClickCallback(const std::function<void()>& callback) { mOnClickCallback = callback; }
void Button::activateOnClickCallback() {
    if (mOnClickCallback)
        mOnClickCallback();
}

void Button::setOnReleaseCallback(const std::function<void()>& callback) { mOnReleaseCallback = callback; }
void Button::activateOnReleaseCallback() {
    if (mOnReleaseCallback)
        mOnReleaseCallback();
}

bool Button::isHovered() const { return mHovered; }
void Button::setHovered(bool aHovered) { mHovered = aHovered; }

void Button::setWidth(float aWidth) { mWidth = aWidth; }
void Button::setHeight(float aHeight) { mHeight = aHeight; }

float Button::getWidth() const { return mWidth; }
float Button::getHeight() const { return mHeight; }

void Button::setInteractable(bool aInteractable) { mInteractable = aInteractable; }
bool Button::interactable() const { return mInteractable; }

BoundingBox Button::getBoundingBox() const {
    BoundingBox box;
    box.topLeft = mTransform.position;
    box.bottomRight = mTransform.position + Vector2(mWidth, mHeight);
    return box;
}
