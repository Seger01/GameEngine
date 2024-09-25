#pragma once
#include "IComponent.h"
#include <iostream>

class PhysicsComponent : public IComponent {
    public:
        PhysicsComponent();
        void ApplyPhysics();
        void Update(double deltaTime);
        const char* GetType() const;

        double getMass() const;
        double getFriction() const;

        void setMass(double);
        void setFriction(double);
    private:
        double mass;
        double friction;
};