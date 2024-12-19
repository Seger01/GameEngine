#ifndef BODYPROXY_H
#define BODYPROXY_H

/**
 * @file BodyProxy.h
 *
 * @brief This file contains the declaration of the BodyProxy class which is used to
 * temporarily store the properties of a body and send it to the world
 *
 */

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

	Vector2 getScale() const;

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
	/// @brief Position of the body
	Vector2 mPosition;
	/// @brief Rotation of the body
	float mRotation;
	/// @brief Scale of the body
	Vector2 mScale;

	/// @brief Whether the body has gravity
	bool mHasGravity;
	/// @brief Whether the body is moveable by force
	bool mIsMoveableByForce;
	/// @brief Whether the body can rotate
	bool mCanRotate;
	/// @brief Whether the body can collide
	bool mCanCollide;
	/// @brief The body type
	BodyType mBodyType;

	/// @brief Linear damping of the body
	float mLinearDamping;
	/// @brief Angular damping of the body
	float mAngularDamping;

	/// @brief Density of the body
	float mDensity;
	/// @brief Friction of the body
	float mFriction;
	/// @brief Restitution of the body
	float mRestitution;

	/// @brief Mass of the body
	float mMass;
	/// @brief Gravity scale of the body
	float mGravityScale;

	/// @brief Box colliders of the body
	std::vector<std::reference_wrapper<BoxCollider>> mBoxColliders;
	/// @brief Circle colliders of the body
	std::vector<std::reference_wrapper<CircleCollider>> mCircleColliders;
};

#endif // BODYPROXY_H
