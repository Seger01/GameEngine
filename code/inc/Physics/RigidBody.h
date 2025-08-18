#pragma once
#include "Component/Component.h"
#include "Physics/BodyType.h"
#include "Structs/BodyFlags.h"
#include "Structs/BodyID.h"
#include "Structs/BodyProperties.h"
#include "Structs/Transform.h"
#include "Structs/Vector2.h"

#include <vector>

/**
 * @file RigidBody.h
 *
 * @brief This file contains the declaration of the RigidBody class which is used to
 * store the properties of a body and send it to the world
 *
 */

class RigidBody : public Component
{
public:
	RigidBody(const BodyFlags& aBodyFlags = {0, 0, 0}, const BodyProperties& aBodyProperties = {0, 0, 0, 0, 0},
			  const std::string& aTag = "defaultRigidBody");
	std::unique_ptr<Component> clone() const override;
	Transform getTransform() const;
	void setTransform(const Transform& aTransform);

	bool getHasGravity() const;
	void setHasGravity(bool aHasGravity);

	bool getIsMoveableByForce() const;
	void setIsMoveableByForce(bool aIsMoveableByForce);

	bool getCanRotate() const;
	void setCanRotate(bool aCanRotate);

	void setLinearDamping(float aLinearDamping);
	float getLinearDamping() const;

	void setAngularDamping(float aAngularDamping);
	float getAngularDamping() const;

	float getDensity() const;
	void setDensity(float aDensity);

	float getFriction() const;
	void setFriction(float aFriction);

	float getRestitution() const;
	void setRestitution(float aRestitution);

	float getMass() const;
	void setMass(float aMass);

	float getGravityScale() const;
	void setGravityScale(float aGravityScale);

	void setBodyType(BodyType aBodyType);
	BodyType getBodyType() const;

	BodyID getBodyId() const;
	void setBodyId(BodyID aBodyID);

	void addForce(Vector2 aForce);
	std::vector<Vector2> getForcesBuffer() const;

	void addTorque(float aTorque);
	std::vector<float> getTorqueBuffer() const;

	void setLinearVelocity(Vector2 aLinearVelocity);
	Vector2 getLinearVelocity() const;

	void setAngularVelocity(float aAngularVelocity);
	float getAngularVelocity() const;

	void setIsUpdated(bool aIsUpdated);
	bool getIsUpdated() const;

	void clearForcesBuffer();
	void clearTorqueBuffer();

private:
	Transform mTransform;

	/// @brief Flags for the body
	bool mIsUpdated;
	/// @brief Whether the body has gravity
	bool mHasGravity;
	/// @brief Whether the body is moveable by force
	bool mIsMoveableByForce;
	/// @brief Whether the body can rotate
	bool mCanRotate;
	/// @brief Whether the body can collide
	bool mCanCollide;
	/// @brief The body type of the body
	BodyType mBodyType;

	/// @brief Reduces linear velocity over time
	float mLinearDamping;
	/// @brief Reduces angular velocity over time
	float mAngularDamping;

	/// @brief The density of the body
	float mDensity;
	/// @brief The friction of the body
	float mFriction;
	/// @brief The restitution of the body
	float mRestitution;

	/// @brief Linear velocity of the body
	Vector2 mLinearVelocity;
	/// @brief Angular velocity of the body
	float mAngularVelocity;
	/// @brief The mass of the body
	float mRotation;

	/// @brief The gravity scale of the body
	float mMass;
	/// @brief The gravity scale of the body
	float mGravityScale;

	/// @brief Box2D ID of the body
	BodyID mBodyID;

	/// @brief Buffer for forces to apply to the body
	std::vector<Vector2> mForcesBuffer;
	/// @brief Buffer for torques to apply to the body
	std::vector<float> mTorqueBuffer;
};
