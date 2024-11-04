#pragma once
#include "BodyType.h"
#include "Component.h"
#include "Structs/BodyFlags.h"
#include "Structs/BodyProperties.h"
#include "Vector2.h"

#include <vector>

class RigidBody : public Component {
public:
    RigidBody(BodyFlags aBodyFlags, BodyProperties aBodyProperties);
    ~RigidBody();

    void setMass(float aMass);
    float getMass() const;

    void processBodyType();
    void setBodyType(BodyType aBodyType);
    BodyType getBodyType() const;
    void setGravityScale(float aGravityScale);
    float getGravityScale() const;

    int getBodyId() const;

private:
    bool mIsUpdated;
    bool mHasGravity;
    bool mIsMoveableByForce;
    bool mCanRotate;
    BodyType mBodyType;

    float mDensity;
    float mFriction;
    float mRestitution;

    float mMass;
    float mGravityScale;

    int mBodyId;

    std::vector<Vector2> mForcesBuffer;
};
