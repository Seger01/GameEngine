#include "Collider.h"

Collider::Collider() {
    // Constructor implementation
}

Collider::~Collider() {
    // Destructor implementation
}

std::unique_ptr<Component> Collider::clone() const { return std::make_unique<Collider>(*this); }

Transform Collider::getTransform() const {
    return mTransform;
}

void Collider::setTransform(const Transform& transform) {
    mTransform = transform;
}