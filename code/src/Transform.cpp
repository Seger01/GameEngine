#include "Transform.h"

// Constructor
Transform::Transform() : mPosition(Point()), mRotation(0.0f), mScale(Point()) {}

// Destructor
Transform::~Transform() {}

// Set position
void Transform::setPosition(Point position) {
    mPosition = position;
}

// Get position
Point Transform::getPosition() {
    return mPosition;
}

// Set rotation
void Transform::setRotation(float rotation) {
    mRotation = rotation;
}

// Get rotation
float Transform::getRotation() {
    return mRotation;
}

// Set scale
void Transform::setScale(Point scale) {
    mScale = scale;
}

// Get scale
Point Transform::getScale() {
    return mScale;
}