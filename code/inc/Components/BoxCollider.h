#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(std::string aTag = "defaultBoxCollider");
	virtual ~BoxCollider();

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
	void setCollideWithCategory(std::vector<int> aCollideWith);

	bool getIsUpdated();
	void setIsUpdated(bool aUpdated);

private:
	float mWidth;
	float mHeight;
	float mRotation;

	bool mIsTrigger;					   /** < Detirmines of the collider is a sensor */
	std::vector<int> mCollideWithCategory; /** < Category with which shapes the collider can collide */
	int mCollideCategory;				   /** < Collision category of the shape */

	bool mIsUpdated;
};
