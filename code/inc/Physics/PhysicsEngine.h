#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

/**
 * @file PhysicsEngine.h
 *
 * @brief This file contains the declaration of the PhysicsEngine class
 *
 */
#include "GameObject.h"
#include "IBehaviourScript.h"
#include "World.h"
#include <vector>

class PhysicsManager; // Forward declaration of PhysicsManager

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();
	float getStep() const;
	float getSubStep() const;
	World& getWorld();

	void setStep(float);
	void setSubStep(int);
	void setGravity(const Vector2& aGravity);

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	void createWorld(const Vector2& aGravity); // The target function

	void createBodies();

	void update();
	void updateFlags();

	void setPositions();
	void applyForces();

	void executeCollisionScripts(const std::vector<std::pair<int, int>>& aBodyIDs);

	void convertToBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects);
	void convertFromBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects);

	GameObject& getGameObjectByID(int aID);

	friend class PhysicsManager; // PhysicsManager can access private methods like createWorld

private:
	/// @brief Vector of game objects
	std::vector<std::reference_wrapper<GameObject>> mObjects;
	/// @brief The world
	World mWorld;

	/// @brief The step of the physics engine
	float mStep;
	/// @brief The substep of the physics engine
	int mSubStep;
};

#endif // PHYSICSENGINE_H

