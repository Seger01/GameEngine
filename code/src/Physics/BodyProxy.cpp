#include "Physics/BodyProxy.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "RigidBody.h"
#include <vector>

BodyProxy::BodyProxy(GameObject* aGameObject) {
    std::vector<RigidBody*> rigidBodies = aGameObject->getComponents<RigidBody>();

    mBoxColliders = aGameObject->getComponents<BoxCollider>();

    // mPosition = aGameObject->getTransform().position;
    Transform transform = aGameObject->getTransform();

    transform.position = transform.position + mBoxColliders[0]->getTransform().position;

    transform.position.x = transform.position.x + mBoxColliders.at(0)->getWidth() / 2;
    transform.position.y = transform.position.y + mBoxColliders.at(0)->getHeight() / 2;

    mPosition = Vector2(-transform.position.x, -transform.position.y);

    mSize = aGameObject->getTransform().scale;

    mHasGravity = rigidBodies.at(0)->getHasGravity();
    mIsMoveableByForce = rigidBodies.at(0)->getIsMoveableByForce();
    mCanRotate = rigidBodies.at(0)->getCanRotate();
    mDensity = rigidBodies.at(0)->getDensity();
    mFriction = rigidBodies.at(0)->getFriction();
    mRestitution = rigidBodies.at(0)->getRestitution();
    mMass = rigidBodies.at(0)->getMass();
    mGravityScale = rigidBodies.at(0)->getGravityScale();
    mLinearDamping = rigidBodies.at(0)->getLinearDamping();
    mAngularDamping = rigidBodies.at(0)->getAngularDamping();

    // processBodyType();
    mBodyType = BodyType::DYNAMIC;

    for (BoxCollider* boxCollider : mBoxColliders) {
        boxCollider->setWidth(boxCollider->getWidth() / 2);
        boxCollider->setHeight(boxCollider->getHeight() / 2);
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

float BodyProxy::getLinearDamping() const { return mLinearDamping; }
float BodyProxy::getAngularDamping() const { return mAngularDamping; }

float BodyProxy::getDensity() const { return mDensity; }
float BodyProxy::getFriction() const { return mFriction; }
float BodyProxy::getRestitution() const { return mRestitution; }

float BodyProxy::getMass() const { return mMass; }
float BodyProxy::getGravityScale() const { return mGravityScale; }

Vector2 BodyProxy::getPosition() const { return mPosition; }
Vector2 BodyProxy::getSize() const { return mSize; }
std::vector<BoxCollider*> BodyProxy::getBoxColliders() const { return mBoxColliders; }
std::vector<CircleCollider*> BodyProxy::getCircleColliders() const { return mCircleColliders; }
