#ifndef BODYPROXY_H
#define BODYPROXY_H

#include "BodyType.h"
#include "Components/BoxCollider.h"
#include "Components/CircleCollider.h"
#include "Components/Collider.h"
#include "GameObject.h"
#include "Point.h"
#include "RigidBody.h"
#include "Vector2.h"
#include <iostream>
#include <vector>
class BodyProxy {
public:
    BodyProxy(GameObject* aGameObject);
    ~BodyProxy();

    void processBodyType();

    Vector2 getPosition() const;
    Vector2 getSize() const;

    bool getHasGravity() const;
    bool getIsMoveableByForce() const;
    bool getCanRotate() const;
    BodyType getBodyType() const;

    float getLinearDamping() const;
    float getAngularDamping() const;

    float getDensity() const;
    float getFriction() const;
    float getRestitution() const;

    float getMass() const;
    float getGravityScale() const;

    std::vector<BoxCollider*> getBoxColliders() const;
    std::vector<CircleCollider*> getCircleColliders() const;

private:
    Vector2 mPosition;
    Vector2 mSize;

    bool mHasGravity;
    bool mIsMoveableByForce;
    bool mCanRotate;
    BodyType mBodyType;

    float mLinearDamping;
    float mAngularDamping;

    float mDensity;
    float mFriction;
    float mRestitution;

    float mMass;
    float mGravityScale;

    std::vector<BoxCollider*> mBoxColliders;
    std::vector<CircleCollider*> mCircleColliders;
};

#endif // BODYPROXY_H
