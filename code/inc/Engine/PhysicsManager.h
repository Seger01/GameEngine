#include "Physics/PhysicsEngine.h"
#include "SceneManager.h"

class PhysicsManager {
public:
    PhysicsManager();
    void startPhysicsEngine(std::vector<GameObject*>&, Vector2 aGravity);
    void updatePhysicsEngine(Scene* aScene);

private:
    PhysicsEngine mPhysicsEngine;
};
