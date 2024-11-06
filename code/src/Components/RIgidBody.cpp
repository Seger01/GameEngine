#include "RigidBody.h"

RigidBody::RigidBody(BodyFlags aBodyFlags, BodyProperties aBodyProperties)
    : mHasGravity(aBodyFlags.HasGravity), mIsMoveableByForce(aBodyFlags.IsMoveableByForce),
      mCanRotate(aBodyFlags.IsMoveableByForce), mDensity(aBodyProperties.Density), mFriction(aBodyProperties.Friction),
      mRestitution(aBodyProperties.Restitution) {}

RigidBody::~RigidBody() {}

bool RigidBody::getHasGravity() const { return mHasGravity; }
void RigidBody::setHasGravity(bool aHasGravity) { mHasGravity = aHasGravity; }

bool RigidBody::getIsMoveableByForce() const { return mIsMoveableByForce; }
void RigidBody::setIsMoveableByForce(bool aIsMoveableByForce) { mIsMoveableByForce = aIsMoveableByForce; }

bool RigidBody::getCanRotate() const { return mCanRotate; }
void RigidBody::setCanRotate(bool aCanRotate) { mCanRotate = aCanRotate; }

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

int RigidBody::getBodyId() const { return mBodyType; }
