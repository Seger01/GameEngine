#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() {}

void PhysicsManager::startPhysicsEngine(const Vector2& aGravity) { mPhysicsEngine.createWorld(aGravity); }

void PhysicsManager::updatePhysicsEngine(double timeDilation)
{
	double step = 0.02 * timeDilation;
	mPhysicsEngine.setStep(step);
	mPhysicsEngine.update();
}

PhysicsEngine& PhysicsManager::getPhysicsEngine() { return mPhysicsEngine; }
