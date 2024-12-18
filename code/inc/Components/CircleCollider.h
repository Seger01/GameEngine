#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

/**
 * @file CircleCollider.h
 *
 * @brief This file contains the declaration of the CircleCollider class
 *
 */

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider(float aRadius = 0.0f, std::string aTag = "defaultCircleCollider");
	~CircleCollider();

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

#endif // CIRCLE_COLLIDER_H
