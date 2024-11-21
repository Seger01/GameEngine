#include "Button.h"

#include <iostream>

Button::Button() : UIObject(), mWidth(0), mHeight(0), mInteractable(true) {}

Button::~Button() {}

void Button::OnClick() { std::cout << "Button Clicked!" << std::endl; }

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
