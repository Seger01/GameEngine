#include "Components/RigidBody.h"

/**
 * @file RigidBody.cpp
 *
 * @brief This file contains the implementation of the RigidBody class which is used to
 */

/**
 * @brief Constructs a RigidBody instance with the specified body flags, body properties, and tag.
 *
 * @param aBodyFlags The flags that define physical behaviors of the body, such as gravity and movability by force.
 * @param aBodyProperties The properties of the body, including density, friction, restitution, gravity scale, and mass.
 * @param aTag A string tag to identify the RigidBody component.
 *
 * @see BodyFlags
 * @see BodyProperties
 */
RigidBody::RigidBody(const BodyFlags& aBodyFlags, const BodyProperties& aBodyProperties, const std::string& aTag)
	: Component{aTag}, mHasGravity(aBodyFlags.HasGravity), mIsMoveableByForce(aBodyFlags.IsMoveableByForce),
	  mCanRotate(aBodyFlags.IsMoveableByForce), mDensity(aBodyProperties.Density), mFriction(aBodyProperties.Friction),
	  mRestitution(aBodyProperties.Restitution), mLinearDamping(0.0f), mAngularDamping(0.0f), mIsUpdated(false),
	  mBodyID({-1, 0}), mAngularVelocity(0.0f), mLinearVelocity({0.0f, 0.0f}),
	  mGravityScale(aBodyProperties.GravityScale), mMass(aBodyProperties.Mass)
{
}

std::unique_ptr<Component> RigidBody::clone() const { return std::make_unique<RigidBody>(*this); }

Transform RigidBody::getTransform() const { return mTransform; }

void RigidBody::setTransform(const Transform& aTransform) { mTransform = aTransform; }

bool RigidBody::getHasGravity() const { return mHasGravity; }

void RigidBody::setHasGravity(bool aHasGravity)
{
	mIsUpdated = true;
	mHasGravity = aHasGravity;
}

bool RigidBody::getIsMoveableByForce() const { return mIsMoveableByForce; }

void RigidBody::setIsMoveableByForce(bool aIsMoveableByForce)
{
	mIsUpdated = true;
	mIsMoveableByForce = aIsMoveableByForce;
}

float RigidBody::getAngularVelocity() const { return mAngularVelocity; }

void RigidBody::setAngularVelocity(float aAngularVelocity) { mAngularVelocity = aAngularVelocity; }

bool RigidBody::getCanRotate() const { return mCanRotate; }

void RigidBody::setCanRotate(bool aCanRotate)
{
	mIsUpdated = true;
	mCanRotate = aCanRotate;
}

void RigidBody::setLinearDamping(float aLinearDamping)
{
	mIsUpdated = true;
	mLinearDamping = aLinearDamping;
}

float RigidBody::getLinearDamping() const { return mLinearDamping; }

void RigidBody::setAngularDamping(float aAngularDamping)
{
	mIsUpdated = true;
	mAngularDamping = aAngularDamping;
}

float RigidBody::getAngularDamping() const { return mAngularDamping; }

float RigidBody::getDensity() const { return mDensity; }

void RigidBody::setDensity(float aDensity)
{
	mIsUpdated = true;
	mDensity = aDensity;
}

float RigidBody::getFriction() const { return mFriction; }

void RigidBody::setFriction(float aFriction)
{
	mIsUpdated = true;
	mFriction = aFriction;
}

float RigidBody::getRestitution() const { return mRestitution; }

void RigidBody::setRestitution(float aRestitution)
{
	mIsUpdated = true;
	mRestitution = aRestitution;
}

float RigidBody::getMass() const { return mMass; }

void RigidBody::setMass(float aMass)
{
	mIsUpdated = true;
	mMass = aMass;
}

float RigidBody::getGravityScale() const { return mGravityScale; }

void RigidBody::setGravityScale(float aGravityScale)
{
	mIsUpdated = true;
	mGravityScale = aGravityScale;
}

void RigidBody::setBodyType(BodyType aBodyType)
{
	mIsUpdated = true;
	mBodyType = aBodyType;
}

Vector2 RigidBody::getLinearVelocity() const { return mLinearVelocity; }

void RigidBody::setLinearVelocity(Vector2 aLinearVelocity) { mLinearVelocity = aLinearVelocity; }

BodyType RigidBody::getBodyType() const { return mBodyType; }

BodyID RigidBody::getBodyId() const { return mBodyID; }

void RigidBody::setBodyId(BodyID aBodyID) { mBodyID = aBodyID; }

void RigidBody::addForce(Vector2 aForce) { mForcesBuffer.push_back(aForce); }

std::vector<Vector2> RigidBody::getForcesBuffer() const { return mForcesBuffer; }

void RigidBody::addTorque(float aTorque) { mTorqueBuffer.push_back(aTorque); }

std::vector<float> RigidBody::getTorqueBuffer() const { return mTorqueBuffer; }

void RigidBody::setIsUpdated(bool aIsUpdated) { mIsUpdated = aIsUpdated; }

bool RigidBody::getIsUpdated() const { return mIsUpdated; }

void RigidBody::clearForcesBuffer() { mForcesBuffer.clear(); }

void RigidBody::clearTorqueBuffer() { mTorqueBuffer.clear(); }
