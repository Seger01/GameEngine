#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "GameObject.h"
#include "IBehaviourScript.h"
#include "Physics/BodyProxy.h"
#include "RigidBody.h"
#include "World.h"
#include "WorldID.h"
#include <vector>
class PhysicsEngine {

public:
    PhysicsEngine();
    ~PhysicsEngine();

    void createWorld(Vector2 aGravity);
    void reset();

    void createBodies();
    void deleteBodies();

    void updateReferences(std::vector<GameObject*>&);
    void update();
    void updateFlags();
    void updateForces();

    void setStep(float);
    void setSubStep(int);

    std::vector<GameObject*> getgameObjects() const;
    float getStep() const;
    float getSubStep() const;
    World& getWorld();
    GameObject* getGameObjectByID(int aID);

    void executeCollisionScripts(std::vector<std::pair<int, int>>);

    void convertFromBox2D(std::vector<GameObject*>& aGameObjects);
    void convertToBox2D(std::vector<GameObject*>& aGameObjects);

private:
    World mWorld;
    WorldID mWorldID;
    std::vector<GameObject*> mGameObjects;

    float mStep;
    int mSubStep;
};

#endif // PHYSICSENGINE_H
