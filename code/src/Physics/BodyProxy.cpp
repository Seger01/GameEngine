#include "Physics/BodyProxy.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "RigidBody.h"
#include <vector>

BodyProxy::BodyProxy(GameObject& aGameObject) {

    std::vector<RigidBody*> rigidBodies = aGameObject.getComponents<RigidBody>();
    if (rigidBodies.at(0) != nullptr) {
        mPosition = aGameObject.getTransform().position;
        mSize = aGameObject.getTransform().scale;

        mHasGravity = rigidBodies.at(0)->getHasGravity();
        mIsMoveableByForce = rigidBodies.at(0)->getIsMoveableByForce();
        mCanRotate = rigidBodies.at(0)->getCanRotate();
        mDensity = rigidBodies.at(0)->getDensity();
        mFriction = rigidBodies.at(0)->getFriction();
        mRestitution = rigidBodies.at(0)->getRestitution();
        mMass = rigidBodies.at(0)->getMass();
        mGravityScale = rigidBodies.at(0)->getGravityScale();

    } else {
        std::cout << "invalid body" << std::endl;
        return;
    }

    mBoxColliders = aGameObject.getComponents<BoxCollider>();
    mCircleColliders = aGameObject.getComponents<CircleCollider>();

    processBodyType();
}

void BodyProxy::processBodyType() {
    if (!mHasGravity && !mIsMoveableByForce && !mCanRotate && mDensity == 0 && mFriction == 0 && mRestitution == 0 &&
        mMass == 0 && mGravityScale == 0) {
        mBodyType = BodyType::STATIC;
    } else if (mHasGravity && mIsMoveableByForce && mCanRotate && mDensity != 0 && mFriction != 0 &&
               mRestitution != 0 && mMass != 0 && mGravityScale != 0) {
        mBodyType = BodyType::DYNAMIC;
    }
}

bool BodyProxy::getHasGravity() const { return mHasGravity; }
bool BodyProxy::getIsMoveableByForce() const { return mIsMoveableByForce; }
bool BodyProxy::getCanRotate() const { return mCanRotate; }
BodyType BodyProxy::getBodyType() const { return mBodyType; }

float BodyProxy::getDensity() const { return mDensity; }
float BodyProxy::getFriction() const { return mFriction; }
float BodyProxy::getRestitution() const { return mRestitution; }

float BodyProxy::getMass() const { return mMass; }
float BodyProxy::getGravityScale() const { return mGravityScale; }
