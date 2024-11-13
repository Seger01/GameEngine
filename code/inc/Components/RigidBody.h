#pragma once
#include "BodyType.h"
#include "Component.h"

class RigidBody : public Component
{
    public:
        RigidBody();
        ~RigidBody();

        std::unique_ptr<Component> clone() const override;

        void setMass(float mass);
        float getMass();
        void setBodyType(BodyType bodyType);
        float getGravityScale();
        void setGravityScale(float gravityScale);
        BodyType getBodyType();
    private:
        float mMass;
        float mGravityScale;
        BodyType mBodyType;
};