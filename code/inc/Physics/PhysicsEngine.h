#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "GameObject.h"
#include "IBehaviourScript.h"
#include "Physics/BodyProxy.h"
#include "RigidBody.h"
#include "World.h"
#include "WorldID.h"
#include <functional>
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

	void setStep(float);
	void setSubStep(int);

	void setPositions();
	void applyForces();

	std::vector<GameObject*> getgameObjects() const;
	float getStep() const;
	float getSubStep() const;
	void setGravity(Vector2 aGravity);
	World& getWorld();
	GameObject* getGameObjectByID(int aID);

	void executeCollisionScripts(std::vector<std::pair<int, int>>);

	void convertFromBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects);
	void convertToBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects);

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
