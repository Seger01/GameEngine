#include "PhysicsManager.h"

void PhysicsManager::startPhysicsEngine() {
    mPhysicsEngine.createWorld(Vector2(0, 0));
    mPhysicsEngine.createBodies();
}
