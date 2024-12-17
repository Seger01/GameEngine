#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "GameObject.h"
#include "IBehaviourScript.h"
#include "World.h"
#include <vector>

class PhysicsEngine
{

public:
	PhysicsEngine();
	~PhysicsEngine();

	void createWorld(Vector2 aGravity);
	void createBodies();

	void update();
	void updateFlags();

	void setPositions();
	void applyForces();

	void executeCollisionScripts(std::vector<std::pair<int, int>>);

	void convertToBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects);
	void convertFromBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects);

	GameObject& getGameObjectByID(int aID);

	float getStep() const;
	float getSubStep() const;
	World& getWorld();

	void setStep(float);
	void setSubStep(int);
	void setGravity(Vector2 aGravity);

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	std::vector<std::reference_wrapper<GameObject>> mObjects;
	World mWorld;

	float mStep;
	int mSubStep;
};

#endif // PHYSICSENGINE_H
