#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "GameObject.h"
#include "IBehaviourScript.h"
#include "Physics/BodyProxy.h"
#include "Physics/World.h"
#include "RigidBody.h"
#include <vector>
class PhysicsEngine {

public:
    PhysicsEngine();

    void updateReferences(std::vector<GameObject*>&);
    void update();
    void updateFlags();
    void updateForces();

    void setSubStep(int);
    float getSubStep() const;
    void setStep(float);
    float getStep() const;
    void setgameObjects(std::vector<GameObject*>);
    std::vector<GameObject*> getgameObjects() const;

    void executeCollisionScripts(std::vector<std::pair<int, int>>);

    void createBodies();

    void createWorld(Vector2 aGravity);

    World& getWorld();

    void reset();

    GameObject* getGameObjectByID(int aID);

    void setCollision(int aBodyID, bool aState);

    GameObject* convertFromBox2D(GameObject* aGameObject);
    GameObject* convertToBox2D(GameObject* aGameObject);

private:
    World mWorld;
    std::vector<GameObject*> mGameObjects;

    float mStep;
    int mSubStep;
};

#endif // PHYSICSENGINE_H
