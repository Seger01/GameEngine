#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	std::unique_ptr<Component> clone() const override;

	// Getter and setter methods for width, height, and rotation
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
	void setCollideWithCategory(std::vector<int> aCollideWith);

	bool getIsUpdated();
	void setIsUpdated(bool aUpdated);

private:
	bool mIsTrigger;
	float mWidth;
	float mHeight;
	float mRotation;
	std::vector<int> mCollideWithCategory;
	int mCollideCategory;
	bool mIsUpdated;
};
