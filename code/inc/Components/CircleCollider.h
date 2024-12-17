#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

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
	void setCollideWithCategory(std::vector<int> aCollideWith);

	std::unique_ptr<Component> clone() const override;

	bool getIsUpdated();
	void setIsUpdated(bool aUpdated);

private:
	float mRadius;

	std::vector<int> mCollideWithCategory; /** < Category with which shapes the collider can collide */
	int mCollideCategory;				   /** < Collision category of the shape */
	bool mIsTrigger;					   /** < Determines of the collider is a sensor */

	bool mIsUpdated;
};

#endif // CIRCLE_COLLIDER_H
