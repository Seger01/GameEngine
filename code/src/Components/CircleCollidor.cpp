#include "Components/CircleCollider.h"

CircleCollider::CircleCollider(float aRadius, std::string aTag)
	: Collider{aTag}, mRadius(aRadius), mIsTrigger(false), mCollideCategory(1), mCollideWithCategory({1}),
	  mIsUpdated(false)
{
}

CircleCollider::~CircleCollider() {}

float CircleCollider::getRadius() const { return mRadius; }
void CircleCollider::setRadius(float radius) { mRadius = radius; }

bool CircleCollider::getTrigger() const { return mIsTrigger; }
void CircleCollider::setTrigger(bool isTrigger) { mIsTrigger = isTrigger; }

int CircleCollider::getCollideCategory() const { return mCollideCategory; }
void CircleCollider::setCollideCategory(int category) { mCollideCategory = category; }

std::vector<int> CircleCollider::getCollideWithCategory() const { return mCollideWithCategory; }
void CircleCollider::setCollideWithCategory(std::vector<int> aCollideWith) { mCollideWithCategory = aCollideWith; }

std::unique_ptr<Component> CircleCollider::clone() const { return std::make_unique<CircleCollider>(*this); }

bool CircleCollider::getIsUpdated() { return mIsUpdated; }
void CircleCollider::setIsUpdated(bool aUpdated) { mIsUpdated = aUpdated; }
