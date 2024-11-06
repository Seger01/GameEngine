#include "Physics/PhysicsEngine.h"
#include "GameObject.h"
#include "Physics/BodyProxy.h"
#include <vector>

void PhysicsEngine::updateReferences(std::vector<GameObject*>& aGameObjects) {}

void PhysicsEngine::update() {}

void PhysicsEngine::setSubStep(int aSubStep) {}

float PhysicsEngine::getSubStep() const { return mSubStep; }

void PhysicsEngine::setStep(float aStep) {}

float PhysicsEngine::getStep() const { return mStep; }

void PhysicsEngine::executeCollisionScripts(std::vector<int> aBodyIDs) {}

void PhysicsEngine::createBodies() {
    for (std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it) {
        BodyProxy bodyProxy = BodyProxy(**it);

        mWorld.createBody(bodyProxy);
    }
}

void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld = World(aGravity); }
