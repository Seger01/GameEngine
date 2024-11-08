#include "Physics/PhysicsEngine.h"

class PhysicsManager {
public:
    PhysicsManager();
    void startPhysicsEngine(std::vector<GameObject*>&, Vector2 aGravity);
    void updatePhysicsEngine();

private:
    PhysicsEngine mPhysicsEngine;
};
