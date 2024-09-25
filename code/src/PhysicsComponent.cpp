#include "PhysicsComponent.h"
#include <iostream>


PhysicsComponent::PhysicsComponent() {}


void PhysicsComponent::Update(double deltaTime) 
{
    std::cout << "Updating Physics Component" << std::endl;
}

void PhysicsComponent::ApplyPhysics() 
{
    std::cout << "Applying Physics" << std::endl;
}

const char* PhysicsComponent::GetType() const{
    return "PhysicsComponent";
}

double PhysicsComponent::getMass() const {
    return mass;
}
double PhysicsComponent::getFriction() const {
    return friction;
}

void PhysicsComponent::setMass(double Mass) {
    mass = Mass;
}

void PhysicsComponent::setFriction(double Friction) {
    friction = Friction;
}