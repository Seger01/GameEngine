#include "Components/RigidBody.h"

std::unique_ptr<Component> RigidBody::clone() const { return std::make_unique<RigidBody>(*this); }