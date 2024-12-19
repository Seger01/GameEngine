#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

/**
 * @file PhysicsManager.h
 *
 * @brief Declaration for the physics manager, used to call update loop and create functions of
 * the physics engine
 *
 */

#include "Physics/PhysicsEngine.h"

class PhysicsManager
{
public:
	PhysicsManager();
	void startPhysicsEngine(const Vector2& aGravity);
	void updatePhysicsEngine(double timeDilation);
	PhysicsEngine& getPhysicsEngine();

private:
	/// @brief The physics engine
	PhysicsEngine mPhysicsEngine;
};

#endif // PHYSICSMANAGER_H
