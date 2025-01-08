#include "Components/CircleCollider.h"

/**
 * @file CircleCollider.cpp
 *
 * @brief This file contains the implementation of the CircleCollider class which is used to
 * store the properties of a circle collider and send it to the world
 *
 */

/**
 * @brief Constructs a CircleCollider instance with the specified radius and tag.
 *
 * @param aRadius The radius of the circle collider.
 * @param aTag A string tag to identify the CircleCollider component.
 */
CircleCollider::CircleCollider(float aRadius, const std::string& aTag)
	: Collider{aTag}, mRadius(aRadius), mIsTrigger(false), mCollideCategory(1), mCollideWithCategory({1}),
	  mIsUpdated(false)
{
}

float CircleCollider::getRadius() const { return mRadius; }

void CircleCollider::setRadius(float radius)
{
	mIsUpdated = true;
	mRadius = radius;
}

bool CircleCollider::getIsTrigger() const { return mIsTrigger; }

void CircleCollider::setIsTrigger(bool isTrigger)
{
	mIsUpdated = true;
	mIsTrigger = isTrigger;
}

int CircleCollider::getCollideCategory() const { return mCollideCategory; }

void CircleCollider::setCollideCategory(int category)
{
	mIsUpdated = true;
	mCollideCategory = category;
}

std::vector<int> CircleCollider::getCollideWithCategory() const { return mCollideWithCategory; }

void CircleCollider::setCollideWithCategory(const std::vector<int>& aCollideWith)
{
	mIsUpdated = true;
	mCollideWithCategory = aCollideWith;
}

std::unique_ptr<Component> CircleCollider::clone() const { return std::make_unique<CircleCollider>(*this); }

bool CircleCollider::getIsUpdated() { return mIsUpdated; }

void CircleCollider::setIsUpdated(bool aUpdated) { mIsUpdated = aUpdated; }
