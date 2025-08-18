#pragma once

/**
 * @file CircleCollider.h
 *
 * @brief This file contains the declaration of the CircleCollider class
 *
 */

#include "Physics/Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider(float aRadius = 0.0f, const std::string& aTag = "defaultCircleCollider");
	float getRadius() const;
	void setRadius(float radius);

	bool getIsTrigger() const;
	void setIsTrigger(bool isTrigger);

	int getCollideCategory() const;
	void setCollideCategory(int category);

	std::vector<int> getCollideWithCategory() const;
	void setCollideWithCategory(const std::vector<int>& aCollideWith);

	std::unique_ptr<Component> clone() const override;

	bool getIsUpdated();
	void setIsUpdated(bool aUpdated);

private:
	/// @brief Radius of the circle
	float mRadius;
	/// @brief Category with which shapes the collider can collide
	std::vector<int> mCollideWithCategory;
	/// @brief Category of the collider
	int mCollideCategory;
	/// @brief Whether the collider is a trigger
	bool mIsTrigger;

	/// @brief Whether the collider has been updated
	bool mIsUpdated;
};
