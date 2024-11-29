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

	bool getTrigger() const;
	void setTrigger(bool isTrigger);

	int getCollideCategory() const;
	void setCollideCategory(int category);

	std::vector<int> getCollideWithCategory() const;
	void setCollideWithCategory(std::vector<int> aCollideWith);

	std::unique_ptr<Component> clone() const override;

private:
	std::vector<int> mCollideWithCategory;
	int mCollideCategory;

	bool mIsTrigger;

	float mRadius;
};

#endif // CIRCLE_COLLIDER_H
