#include "BoxCollider.h"

BoxCollider::BoxCollider()
    : mWidth(0.0f), mHeight(0.0f), mRotation(0.0f) {
    // Constructor implementation
}

BoxCollider::~BoxCollider() {
    // Destructor implementation
}

float BoxCollider::getWidth() const {
    return mWidth;
}

void BoxCollider::setWidth(float width) {
    mWidth = width;
}

float BoxCollider::getHeight() const {
    return mHeight;
}

void BoxCollider::setHeight(float height) {
    mHeight = height;
}

float BoxCollider::getRotation() const {
    return mRotation;
}

void BoxCollider::setRotation(float rotation) {
    mRotation = rotation;
}

void BoxCollider::setTransformFromColliderData(const ColliderData& colliderData) {
    Transform transform;
    transform.position.x = colliderData.x;
    transform.position.y = colliderData.y;
    setTransform(transform);
    setWidth(colliderData.mWidth);
    setHeight(colliderData.mHeight);
}