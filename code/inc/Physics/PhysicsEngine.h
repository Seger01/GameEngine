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

    void createWorld(Vector2 aGravity);
    void reset();

    void createBodies();

    void updateReferences(std::vector<GameObject*>&);
    void update();
    void updateFlags();
    void updateForces();

    void setgameObjects(std::vector<GameObject*>);
    void setStep(float);
    void setSubStep(int);

    std::vector<GameObject*> getgameObjects() const;
    float getStep() const;
    float getSubStep() const;
    World& getWorld();
    GameObject* getGameObjectByID(int aID);

    void executeCollisionScripts(std::vector<std::pair<int, int>>);

    void convertFromBox2D(GameObject* aGameObject);
    void convertToBox2D(GameObject* aGameObject);

private:
    World mWorld;
    std::vector<GameObject*> mGameObjects;

    float mStep;
    int mSubStep;
};

#endif // PHYSICSENGINE_H
