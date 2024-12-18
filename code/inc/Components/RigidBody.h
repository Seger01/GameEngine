#pragma once
#include "BodyID.h"
#include "BodyType.h"
#include "Component.h"
#include "Structs/BodyFlags.h"
#include "Structs/BodyProperties.h"
#include "Transform.h"
#include "Vector2.h"

#include <vector>

class RigidBody : public Component
{
public:
	RigidBody(BodyFlags aBodyFlags = {0, 0, 0}, BodyProperties aBodyProperties = {0, 0, 0, 0, 0},
			  std::string aTag = "defaultRigidBody");
	~RigidBody();
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

	bool mIsUpdated;
	bool mHasGravity;
	bool mIsMoveableByForce;
	bool mCanRotate;
	bool mCanCollide;
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
