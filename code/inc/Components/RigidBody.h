#pragma once
#include "BodyType.h"
#include "Component.h"
#include "Structs/BodyFlags.h"
#include "Structs/BodyProperties.h"
#include "Transform.h"
#include "Vector2.h"

#include <vector>

class RigidBody : public Component {
public:
    RigidBody(BodyFlags aBodyFlags, BodyProperties aBodyProperties);
    RigidBody();
    ~RigidBody();

    Transform getTransform() const;
    void setTransform(const Transform& aTransform);

    bool getHasGravity() const;
    void setHasGravity(bool aHasGravity);

    bool getIsMoveableByForce() const;
    void setIsMoveableByForce(bool aIsMoveableByForce);

    bool getCanRotate() const;
    void setCanRotate(bool aCanRotate);

    float getDensity() const;
    void setDensity(float aDensity);

    float getFriction() const;
    void setFriction(float aFriction);

    float getRestitution() const;
    void setRestitution(float aRestitution);

    float getMass() const;
    void setMass(float aMass);

    float getGravityScale() const;
    void setGravityScale(float aGravityScale);

    void setBodyType(BodyType aBodyType);
    BodyType getBodyType() const;

    int getBodyId() const;

private:
    Transform mTransform;

    bool mIsUpdated;
    bool mHasGravity = false;
    bool mIsMoveableByForce = false;
    bool mCanRotate = false;
    BodyType mBodyType;

    float mDensity = 0;
    float mFriction = 0;
    float mRestitution = 0;

    float mMass = 0;
    float mGravityScale = 0;

    int mBodyId;

    std::vector<Vector2> mForcesBuffer;
};
