#ifndef BODYPROXY_H
#define BODYPROXY_H

#include "BodyType.h"
#include "Components/BoxCollider.h"
#include "Components/CircleCollider.h"
#include "Components/Collider.h"

#include "GameObject.h"
#include "RigidBody.h"
#include "Vector2.h"
#include <iostream>
#include <vector>

class BodyProxy
{
public:
	BodyProxy(const std::reference_wrapper<GameObject>& aGameObject);
	~BodyProxy();

	void processBodyType();

	Vector2 getPosition() const;
	float getRotation() const;

	bool getHasGravity() const;
	bool getIsMoveableByForce() const;
	bool getCanRotate() const;
	BodyType getBodyType() const;

	float getLinearDamping() const;
	float getAngularDamping() const;

	float getDensity() const;
	float getFriction() const;
	float getRestitution() const;

	float getMass() const;
	float getGravityScale() const;

	bool getCanCollide() const;

	std::vector<std::reference_wrapper<BoxCollider>> getBoxColliders() const;
	std::vector<std::reference_wrapper<CircleCollider>> getCircleColliders() const;

private:
	Vector2 mPosition;
	float mRotation;

	bool mHasGravity;
	bool mIsMoveableByForce;
	bool mCanRotate;
	bool mCanCollide;
	BodyType mBodyType;

	float mLinearDamping;
	float mAngularDamping;

	float mDensity;
	float mFriction;
	float mRestitution;

	float mMass;
	float mGravityScale;

	std::vector<std::reference_wrapper<BoxCollider>> mBoxColliders;
	std::vector<std::reference_wrapper<CircleCollider>> mCircleColliders;
};

#endif // BODYPROXY_H
