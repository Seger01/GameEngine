#include "Components/CircleCollider.h"

CircleCollider::CircleCollider() : mRadius(0.0f) {}

CircleCollider::~CircleCollider() {}

std::unique_ptr<Component> CircleCollider::clone() const { return std::make_unique<CircleCollider>(*this); }