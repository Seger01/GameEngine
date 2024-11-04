#include "RigidBody.h"

RigidBody::RigidBody(BodyFlags aBodyFlags, BodyProperties aBodyProperties)
    : mHasGravity(aBodyFlags.HasGravity), mIsMoveableByForce(aBodyFlags.IsMoveableByForce),
      mCanRotate(aBodyFlags.IsMoveableByForce), mDensity(aBodyProperties.Density), mFriction(aBodyProperties.Friction),
      mRestitution(aBodyProperties.Restitution) {}

RigidBody::~RigidBody() {}

void RigidBody::processBodyType() {}

void RigidBody::setMass(float aMass) { mMass = aMass; }

float RigidBody::getMass() const { return mMass; }

void RigidBody::setBodyType(BodyType aBodyType) { mBodyType = aBodyType; }

BodyType RigidBody::getBodyType() const { return mBodyType; }

void RigidBody::setGravityScale(float aGravityScale) { mGravityScale = aGravityScale; }

float RigidBody::getGravityScale() const { return mGravityScale; }

int RigidBody::getBodyId() const { return mBodyType; }
