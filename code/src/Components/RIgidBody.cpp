#include "RigidBody.h"
#include <iostream>

RigidBody::RigidBody(BodyFlags aBodyFlags, BodyProperties aBodyProperties, std::string aTag)
    : Component{aTag}, mHasGravity(aBodyFlags.HasGravity), mIsMoveableByForce(aBodyFlags.IsMoveableByForce),
      mCanRotate(aBodyFlags.IsMoveableByForce), mDensity(aBodyProperties.Density), mFriction(aBodyProperties.Friction),
      mRestitution(aBodyProperties.Restitution) {}

RigidBody::~RigidBody() {}

std::unique_ptr<Component> RigidBody::clone() const { return std::make_unique<RigidBody>(*this); }

Transform RigidBody::getTransform() const { return mTransform; }
void RigidBody::setTransform(const Transform& aTransform) { mTransform = aTransform; }

bool RigidBody::getHasGravity() const { return mHasGravity; }
void RigidBody::setHasGravity(bool aHasGravity) { mHasGravity = aHasGravity; }

bool RigidBody::getIsMoveableByForce() const { return mIsMoveableByForce; }
void RigidBody::setIsMoveableByForce(bool aIsMoveableByForce) { mIsMoveableByForce = aIsMoveableByForce; }

bool RigidBody::getCanRotate() const { return mCanRotate; }
void RigidBody::setCanRotate(bool aCanRotate) { mCanRotate = aCanRotate; }

void RigidBody::setLinearDamping(float aLinearDamping) { mLinearDamping = aLinearDamping; }
float RigidBody::getLinearDamping() const { return mLinearDamping; }

void RigidBody::setAngularDamping(float aAngularDamping) { mAngularDamping = aAngularDamping; }
float RigidBody::getAngularDamping() const { return mAngularDamping; }

float RigidBody::getDensity() const { return mDensity; }
void RigidBody::setDensity(float aDensity) { mDensity = aDensity; }

float RigidBody::getFriction() const { return mFriction; }
void RigidBody::setFriction(float aFriction) { mFriction = aFriction; }

float RigidBody::getRestitution() const { return mRestitution; }
void RigidBody::setRestitution(float aRestitution) { mRestitution = aRestitution; }

float RigidBody::getMass() const { return mMass; }
void RigidBody::setMass(float aMass) { mMass = aMass; }

float RigidBody::getGravityScale() const { return mGravityScale; }
void RigidBody::setGravityScale(float aGravityScale) { mGravityScale = aGravityScale; }

void RigidBody::setBodyType(BodyType aBodyType) { mBodyType = aBodyType; }

BodyType RigidBody::getBodyType() const { return mBodyType; }

int RigidBody::getBodyId() const { return mBodyId; }
void RigidBody::setBodyId(int aBodyId) { mBodyId = aBodyId; }

void RigidBody::addForce(Vector2 aForce) { mForcesBuffer.push_back(aForce); }
std::vector<Vector2> RigidBody::getForcesBuffer() const { return mForcesBuffer; }

void RigidBody::addTorque(float aTorque) { mTorqueBuffer.push_back(aTorque); }
std::vector<float> RigidBody::getTorqueBuffer() const { return mTorqueBuffer; }

void RigidBody::clearForcesBuffer() { mForcesBuffer.clear(); }
void RigidBody::clearTorqueBuffer() { mTorqueBuffer.clear(); }



