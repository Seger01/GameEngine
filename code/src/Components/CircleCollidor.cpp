#include "Components/CircleCollider.h"

CircleCollider::CircleCollider(float aRadius, std::string aTag) : Collider{aTag}, mRadius(aRadius) {}

CircleCollider::~CircleCollider() {}

float CircleCollider::getRadius() const { return mRadius; }
void CircleCollider::setRadius(float radius) { mRadius = radius; }

std::unique_ptr<Component> CircleCollider::clone() const { return std::make_unique<CircleCollider>(*this); }
