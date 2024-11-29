#include "BoxCollider.h"

BoxCollider::BoxCollider()
	: mWidth(0.0f), mHeight(0.0f), mRotation(0.0f), mIsTrigger(false), mCollideWithCategory({1, 2}), mCollideCategory(1)
{
	// Constructor implementation
}

BoxCollider::~BoxCollider()
{
	// Destructor implementation
}

std::unique_ptr<Component> BoxCollider::clone() const { return std::make_unique<BoxCollider>(*this); }

float BoxCollider::getWidth() const { return mWidth; }

void BoxCollider::setWidth(float width)
{
	mWidth = width;
	mIsUpdated = true;
}

float BoxCollider::getHeight() const { return mHeight; }

void BoxCollider::setHeight(float height)
{
	mHeight = height;
	mIsUpdated = true;
}

float BoxCollider::getRotation() const { return mRotation; }

void BoxCollider::setRotation(float rotation)
{
	mRotation = rotation;
	mIsUpdated = true;
}

bool BoxCollider::isTrigger() const { return mIsTrigger; }

void BoxCollider::setTrigger(bool isTrigger)
{
	mIsTrigger = isTrigger;
	mIsUpdated = true;
}

int BoxCollider::getCollideCategory() const { return mCollideCategory; }

void BoxCollider::setCollideCategory(int category)
{
	mCollideCategory = category;
	mIsUpdated = true;
}

std::vector<int> BoxCollider::getCollideWithCategory() const { return mCollideWithCategory; }

void BoxCollider::setCollideWithCategory(std::vector<int> aCollideWith)
{
	mCollideWithCategory = aCollideWith;
	mIsUpdated = true;
}

bool BoxCollider::getIsUpdated() { return mIsUpdated; }

void BoxCollider::setIsUpdated(bool aUpdated) { mIsUpdated = aUpdated; }
