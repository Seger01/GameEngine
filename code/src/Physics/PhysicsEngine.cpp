#include "Physics/PhysicsEngine.h"
#include "GameObject.h"
#include "Physics/BodyProxy.h"
#include <vector>

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::updateReferences(std::vector<GameObject*>& aGameObjects) { mGameObjects = aGameObjects; }

void PhysicsEngine::update() {
    for (int i = 0; i < mGameObjects.size(); i++) {

        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();

        if (!rigidBodies.empty()) {
            BodyProxy bodyProxy = BodyProxy(*mGameObjects.at(i));
            int id = rigidBodies[0]->getBodyId();

            mWorld.updateBody(id, bodyProxy);
        }
    }
    mWorld.executeWorldStep(mStep, mSubStep);
    mWorld.getContactEvents();
    for (int i = 0; i < mGameObjects.size(); i++) {
        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();

        if (!rigidBodies.empty()) {
            Vector2 position = mWorld.getPosition(rigidBodies.at(0)->getBodyId());
            Transform transform = mGameObjects.at(i)->getTransform();
            transform.position = position;
            mGameObjects.at(i)->setTransform(transform);
        }
    }
    mWorld.applyForce(1, Vector2(100, 100));
}

void PhysicsEngine::setSubStep(int aSubStep) {}

float PhysicsEngine::getSubStep() const { return mSubStep; }

void PhysicsEngine::setStep(float aStep) {}

float PhysicsEngine::getStep() const { return mStep; }

void PhysicsEngine::executeCollisionScripts(std::vector<int> aBodyIDs) {}

void PhysicsEngine::createBodies() {
    for (int i = 0; i < mGameObjects.size(); i++) {
        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();

        BodyProxy bodyProxy = BodyProxy(*mGameObjects.at(i));

        if (bodyProxy.getvalidBody()) {
            int bodyID = mWorld.createBody(bodyProxy);
            rigidBodies[0]->setBodyId(bodyID);
        }
    }
}

void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld.createWorld(aGravity); }

void PhysicsEngine::setgameObjects(std::vector<GameObject*> aGameObjects) { mGameObjects = aGameObjects; }
