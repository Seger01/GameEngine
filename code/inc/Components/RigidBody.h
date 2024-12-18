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

	float mLinearDamping;  /** < Reduces linear velocity over time */
	float mAngularDamping; /** < Reduces angular velocity over time */

	float mDensity;
	float mFriction;
	float mRestitution;

	Vector2 mLinearVelocity;
	float mAngularVelocity;
	float mRotation;

	float mMass;
	float mGravityScale;

	BodyID mBodyID;

	std::vector<Vector2> mForcesBuffer;
	std::vector<float> mTorqueBuffer;
};
