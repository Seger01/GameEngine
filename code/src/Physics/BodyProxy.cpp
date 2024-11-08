#include "Physics/BodyProxy.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "RigidBody.h"
#include <vector>

BodyProxy::BodyProxy(GameObject* aGameObject) {

    std::vector<RigidBody*> rigidBodies = aGameObject->getComponents<RigidBody>();

    if (!rigidBodies.empty()) {
        mBoxColliders = aGameObject->getComponents<BoxCollider>();

        mPosition = aGameObject->getTransform().position;

        mSize = aGameObject->getTransform().scale;

        mHasGravity = rigidBodies.at(0)->getHasGravity();
        mIsMoveableByForce = rigidBodies.at(0)->getIsMoveableByForce();
        mCanRotate = rigidBodies.at(0)->getCanRotate();
        mDensity = rigidBodies.at(0)->getDensity();
        mFriction = rigidBodies.at(0)->getFriction();
        mRestitution = rigidBodies.at(0)->getRestitution();
        mMass = rigidBodies.at(0)->getMass();
        mGravityScale = rigidBodies.at(0)->getGravityScale();

        // processBodyType();
        mBodyType = BodyType::DYNAMIC;

        setvalidBody(true);

    } else {
        return;
    }
}

BodyProxy::~BodyProxy() {}

void BodyProxy::processBodyType() {
    if (!mHasGravity && !mIsMoveableByForce && !mCanRotate && mDensity == 0 && mFriction == 0 && mRestitution == 0 &&
        mMass == 0 && mGravityScale == 0) {
        mBodyType = BodyType::STATIC;
    } else {
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

Vector2 BodyProxy::getPosition() const { return mPosition; }
Vector2 BodyProxy::getSize() const { return mSize; }
std::vector<BoxCollider*> BodyProxy::getBoxColliders() const { return mBoxColliders; }
std::vector<CircleCollider*> BodyProxy::getCircleColliders() const { return mCircleColliders; }

void BodyProxy::setvalidBody(bool aValidBody) { mValidBody = aValidBody; }
bool BodyProxy::getvalidBody() const { return mValidBody; }
