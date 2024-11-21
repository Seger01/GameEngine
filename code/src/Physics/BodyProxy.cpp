#include "Physics/BodyProxy.h"

BodyProxy::BodyProxy(GameObject* aGameObject) {
    if (aGameObject->hasComponent<RigidBody>()) {
        RigidBody* rigidBody = aGameObject->getComponents<RigidBody>().at(0);
        mBoxColliders = aGameObject->getComponents<BoxCollider>();

        Transform transform = aGameObject->getTransform();

        transform.position = transform.position + mBoxColliders[0]->getTransform().position;

        transform.position.x = transform.position.x + mBoxColliders.at(0)->getWidth() / 2;
        transform.position.y = transform.position.y + mBoxColliders.at(0)->getHeight() / 2;

        mPosition = Vector2(-transform.position.x, -transform.position.y);

        mSize = aGameObject->getTransform().scale;

        mHasGravity = rigidBody->getHasGravity();
        mIsMoveableByForce = rigidBody->getIsMoveableByForce();
        mCanRotate = rigidBody->getCanRotate();
        mDensity = rigidBody->getDensity();
        mFriction = rigidBody->getFriction();
        mRestitution = rigidBody->getRestitution();
        mMass = rigidBody->getMass();
        mGravityScale = rigidBody->getGravityScale();
        mLinearDamping = rigidBody->getLinearDamping();
        mAngularDamping = rigidBody->getAngularDamping();

        processBodyType();

        for (BoxCollider* boxCollider : mBoxColliders) {
            boxCollider->setWidth(boxCollider->getWidth() / 2);
            boxCollider->setHeight(boxCollider->getHeight() / 2);
        }

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
