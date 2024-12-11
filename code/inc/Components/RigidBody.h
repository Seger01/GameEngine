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

	Vector2 getLinearVelocity() const;
	void setLinearVelocity(Vector2 aVelocity);

	float getAngularVelocity() const;
	void setAngularVelocity(float aAngularVelocity);

	void setBodyType(BodyType aBodyType);
	BodyType getBodyType() const;

	BodyID getBodyId() const;
	void setBodyId(BodyID aBodyID);

	void addForce(Vector2 aForce);
	std::vector<Vector2> getForcesBuffer() const;

	void addTorque(float aTorque);
	std::vector<float> getTorqueBuffer() const;

	void setIsUpdated(bool aIsUpdated);
	bool getIsUpdated() const;

	void clearForcesBuffer();
	void clearTorqueBuffer();

private:
	Transform mTransform;

	bool mIsUpdated;
	bool mHasGravity = false;
	bool mIsMoveableByForce = false;
	bool mCanRotate = false;
	bool mCanCollide = true;
	BodyType mBodyType;

	float mLinearDamping = 0;  // reduces linear velocity over time
	float mAngularDamping = 0; // reduces angular velocity over time

	float mDensity = 0;
	float mFriction = 0;
	float mRestitution = 0;

	float mMass = 0;
	float mGravityScale = 0;

	Vector2 mLinearVelocity;
	float mAngularVelocity;

	BodyID mBodyID;

	std::vector<Vector2> mForcesBuffer;
	std::vector<float> mTorqueBuffer;
};
