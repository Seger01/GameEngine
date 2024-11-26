#include "BoxCollider.h"

BoxCollider::BoxCollider() : mWidth(0.0f), mHeight(0.0f), mRotation(0.0f), mIsTrigger(false) {
    // Constructor implementation
}

BoxCollider::~BoxCollider() {
    // Destructor implementation
}

std::unique_ptr<Component> BoxCollider::clone() const { return std::make_unique<BoxCollider>(*this); }

float BoxCollider::getWidth() const { return mWidth; }

void BoxCollider::setWidth(float width) { mWidth = width; }

float BoxCollider::getHeight() const { return mHeight; }

void BoxCollider::setHeight(float height) { mHeight = height; }

float BoxCollider::getRotation() const { return mRotation; }

void BoxCollider::setRotation(float rotation) { mRotation = rotation; }

bool BoxCollider::isTrigger() const { return mIsTrigger; }

void BoxCollider::setTrigger(bool isTrigger) { mIsTrigger = isTrigger; }
