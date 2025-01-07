#include "PhysicsManager.h"

/// @brief Constructs a PhysicsManager instance

/**
 * @brief Constructs a PhysicsManager instance
 */
PhysicsManager::PhysicsManager() {}

/**
 * @brief Starts the physics engine
 * @param aGravity The gravity of the world
 */
void PhysicsManager::startPhysicsEngine(const Vector2& aGravity) { mPhysicsEngine.createWorld(aGravity); }

/**
 * @brief Updates the physics engine
 * @param timeDilation The time dilation
 */
void PhysicsManager::updatePhysicsEngine(double timeDilation)
{
	double step = 0.02 * timeDilation;
	mPhysicsEngine.setStep(step);
	mPhysicsEngine.setSubStep(4);
	mPhysicsEngine.update();
}

PhysicsEngine& PhysicsManager::getPhysicsEngine() { return mPhysicsEngine; }
