#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(const std::string& aTag = "defaultBoxCollider");

	std::unique_ptr<Component> clone() const override;

	float getWidth() const;
	void setWidth(float width);

	float getHeight() const;
	void setHeight(float height);

	float getRotation() const;
	void setRotation(float rotation);

	bool isTrigger() const;
	void setTrigger(bool isTrigger);

	int getCollideCategory() const;
	void setCollideCategory(int category);

	std::vector<int> getCollideWithCategory() const;
	void setCollideWithCategory(const std::vector<int>& aCollideWith);

	bool getIsUpdated();
	void setIsUpdated(bool aUpdated);

private:
	/// @brief Width of the box
	float mWidth;
	/// @brief Height of the box
	float mHeight;
	/// @brief Rotation of the box
	float mRotation;

	/// @brief Whether the collider is a trigger
	bool mIsTrigger;
	/// @brief Category with which shapes the collider can collide
	std::vector<int> mCollideWithCategory;
	/// @brief Category of the collider
	int mCollideCategory;

	/// @brief Whether the collider has been updated
	bool mIsUpdated;
};
