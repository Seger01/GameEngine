#include "Components/CircleCollider.h"

CircleCollider::CircleCollider() : mRadius(0.0f) {}

CircleCollider::CircleCollider(float radius) : mRadius(radius) {}

CircleCollider::~CircleCollider() {}

float CircleCollider::getRadius() const { return mRadius; }
void CircleCollider::setRadius(float radius) { mRadius = radius; }

int CircleCollider::getCollideCategory() const { return mCollideCategory; }
void CircleCollider::setCollideCategory(int category) { mCollideCategory = category; }

std::unique_ptr<Component> CircleCollider::clone() const { return std::make_unique<CircleCollider>(*this); }
