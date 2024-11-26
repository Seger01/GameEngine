#include "PhysicsManager.h"
#include "SceneManager.h"

PhysicsManager::PhysicsManager() {}

void PhysicsManager::startPhysicsEngine(Vector2 aGravity) {
    mPhysicsEngine.createWorld(aGravity);

    mPhysicsEngine.setStep(1.0f / 60.0f);
    mPhysicsEngine.setSubStep(6);
}

void PhysicsManager::updatePhysicsEngine(Scene* aScene) { mPhysicsEngine.update(); }

PhysicsEngine& PhysicsManager::getPhysicsEngine() { return mPhysicsEngine; }
