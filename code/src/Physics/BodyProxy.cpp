#include "Physics/BodyProxy.h"
#include <functional>

BodyProxy::BodyProxy(const std::reference_wrapper<GameObject>& aGameObject)
{
	RigidBody& rigidBody = aGameObject.get().getComponents<RigidBody>().at(0);

	if (aGameObject.get().hasComponent<BoxCollider>())
	{
		mBoxColliders = aGameObject.get().getComponents<BoxCollider>();
	}
	if (aGameObject.get().hasComponent<CircleCollider>())
	{
		mCircleColliders = aGameObject.get().getComponents<CircleCollider>();
	}

	mPosition = aGameObject.get().getTransform().position;
	mScale = aGameObject.get().getTransform().scale;
	mRotation = aGameObject.get().getTransform().rotation;
	mHasGravity = rigidBody.getHasGravity();
	mIsMoveableByForce = rigidBody.getIsMoveableByForce();
	mCanRotate = rigidBody.getCanRotate();
	mDensity = rigidBody.getDensity();
	mFriction = rigidBody.getFriction();
	mRestitution = rigidBody.getRestitution();
	mMass = rigidBody.getMass();
	mGravityScale = rigidBody.getGravityScale();
	mLinearDamping = rigidBody.getLinearDamping();
	mAngularDamping = rigidBody.getAngularDamping();

	processBodyType();
}

BodyProxy::~BodyProxy() {}

/**
 * @brief Assigns box2d body type depending on params
 */
void BodyProxy::processBodyType()
{
	if (!mHasGravity && !mIsMoveableByForce && !mCanRotate && mDensity == 0 && mRestitution == 0 && mMass == 0 &&
		mGravityScale == 0)
	{
		mBodyType = BodyType::STATIC;
	}
	else
	{
		mBodyType = BodyType::DYNAMIC;
	}
}

Vector2 BodyProxy::getScale() const { return mScale; }

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

std::vector<std::reference_wrapper<GameObject>> mObjects;

Vector2 BodyProxy::getPosition() const { return mPosition; }

std::vector<std::reference_wrapper<BoxCollider>> BodyProxy::getBoxColliders() const { return mBoxColliders; }

std::vector<std::reference_wrapper<CircleCollider>> BodyProxy::getCircleColliders() const { return mCircleColliders; }

float BodyProxy::getRotation() const { return mRotation; }
