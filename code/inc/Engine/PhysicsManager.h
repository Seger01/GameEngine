#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

/**
 * @file PhysicsManager.h
 *
 * @brief This file contains the declaration of the PhysicsManager class
 *
 */

#include "Physics/PhysicsEngine.h"

class PhysicsManager
{
public:
	PhysicsManager();
	void startPhysicsEngine(const Vector2& aGravity);
	void updatePhysicsEngine(double timePassed);
	PhysicsEngine& getPhysicsEngine();

private:
	PhysicsEngine mPhysicsEngine;
};

#endif // PHYSICSMANAGER_H
