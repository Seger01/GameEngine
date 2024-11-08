#include "Physics/BodyProxy.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "RigidBody.h"
#include <vector>

BodyProxy::BodyProxy(GameObject& aGameObject) {
    std::cout << aGameObject.getName() << std::endl;

    std::vector<RigidBody*> rigidBodies = aGameObject.getComponents<RigidBody>();
    std::cout << "rigid bodies size: " << rigidBodies.size() << std::endl;
    std::cout << "created body proxy" << std::endl;

    if (!rigidBodies.empty()) {

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

    // mCircleColliders = aGameObject.getComponents<CircleCollider>();

    processBodyType();
    std::cout << "body type: " << static_cast<int>(mBodyType) << std::endl;
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

Vector2 BodyProxy::getPosition() const { return mPosition; }
Vector2 BodyProxy::getSize() const { return mSize; }
std::vector<BoxCollider*> BodyProxy::getBoxColliders() const { return mBoxColliders; }
std::vector<CircleCollider*> BodyProxy::getCircleColliders() const { return mCircleColliders; }
