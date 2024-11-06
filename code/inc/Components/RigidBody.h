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
