#include "Collider.h"

Collider::Collider() {
    // Constructor implementation
}

Collider::~Collider() {
    // Destructor implementation
}

Transform Collider::getTransform() const {
    return mTransform;
}

void Collider::setTransform(const Transform& transform) {
    mTransform = transform;
}