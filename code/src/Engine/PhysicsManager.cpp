#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() {}

void PhysicsManager::startPhysicsEngine(Vector2 aGravity)
{
	float timeStep = 20.0f / 60.0f;

	mPhysicsEngine.setStep(timeStep);
	mPhysicsEngine.setSubStep(4);

	mPhysicsEngine.createWorld(aGravity);
}

void PhysicsManager::updatePhysicsEngine() { mPhysicsEngine.update(); }

PhysicsEngine& PhysicsManager::getPhysicsEngine() { return mPhysicsEngine; }
