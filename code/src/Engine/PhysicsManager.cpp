#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() {}

void PhysicsManager::startPhysicsEngine(std::vector<GameObject*>& aGameObjects, Vector2 aGravity) {
    mPhysicsEngine.setgameObjects(aGameObjects);
    mPhysicsEngine.createWorld(aGravity);
    mPhysicsEngine.createBodies();
}

void PhysicsManager::updatePhysicsEngine() { mPhysicsEngine.update(); }
