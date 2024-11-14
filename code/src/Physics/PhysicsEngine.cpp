#include "Physics/PhysicsEngine.h"
#include "GameObject.h"
#include "IBehaviourScript.h"
#include "Input.h"
#include "Physics/BodyProxy.h"
//#include "PlayerBehaviourScript.h"
#include <vector>

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::updateReferences(std::vector<GameObject*>& aGameObjects) { mGameObjects = aGameObjects; }

void PhysicsEngine::update() {
    createBodies();

    for (int i = 0; i < mGameObjects.size(); i++) {

        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();

        if (!rigidBodies.empty()) {
            BodyProxy bodyProxy = BodyProxy(mGameObjects.at(i));
            int id = rigidBodies[0]->getBodyId();

            mWorld.updateBody(id, bodyProxy);
        }
    }
    float timeStep = 20.0f / 60.0f;
    int velocityIterations = 4;
    int positionIterations = 2;

    for (int i = 0; i < mGameObjects.size(); i++) {
        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();
        if (!rigidBodies.empty()) {

            for (int i = 0; i < rigidBodies.size(); i++) {
                mWorld.applyLinearForce(rigidBodies[i]->getBodyId(), rigidBodies[i]->getForcesBuffer());
                rigidBodies[i]->clearForcesBuffer();
            }
        }
    }

    mWorld.executeWorldStep(timeStep, velocityIterations);

    executeCollisionScripts(mWorld.getContactEvents());
    // mWorld.executeWorldStep(mStep, mSubStep);
    for (int i = 0; i < mGameObjects.size(); i++) {
        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();

        if (!rigidBodies.empty()) {
            if (mGameObjects.at(i)->hasComponent<BoxCollider>()) {
                BoxCollider* boxCollider = mGameObjects.at(i)->getComponents<BoxCollider>()[0];

                boxCollider->setWidth(boxCollider->getWidth() * 2);
                boxCollider->setHeight(boxCollider->getHeight() * 2);

                Vector2 position = mWorld.getPosition(rigidBodies.at(0)->getBodyId());
                Transform transform = mGameObjects.at(i)->getTransform();

                transform.position = position;

                transform = Transform(Vector2(-position.x, -position.y));

                transform.position.x = transform.position.x - boxCollider->getWidth() / 2;
                transform.position.y = transform.position.y - boxCollider->getHeight() / 2;

                transform.position = transform.position - boxCollider->getTransform().position;

                mGameObjects.at(i)->setTransform(transform);
            }
        }
    }
}

void PhysicsEngine::setSubStep(int aSubStep) {}

float PhysicsEngine::getSubStep() const { return mSubStep; }

void PhysicsEngine::setStep(float aStep) {}

float PhysicsEngine::getStep() const { return mStep; }

void PhysicsEngine::executeCollisionScripts(std::vector<std::pair<int, int>> aBodyIDs) {
    for (int i = 0; i < aBodyIDs.size(); i++) {
        GameObject* gameObject = getGameObjectByID(aBodyIDs.at(i).first);
        if (gameObject != nullptr) {
            if (gameObject->hasComponent<IBehaviourScript>()) {
                std::vector<IBehaviourScript*> behaviourScript = gameObject->getComponents<IBehaviourScript>();
                GameObject* otherGameObject = getGameObjectByID(aBodyIDs.at(i).second);
                if (otherGameObject != nullptr) {
                    behaviourScript.at(0)->onCollide(otherGameObject);
                }
            }
        }
    }
}

void PhysicsEngine::createBodies() {
    for (int gameObjectIndex = 0; gameObjectIndex < mGameObjects.size(); gameObjectIndex++) {

        std::vector<RigidBody*> rigidBodies = mGameObjects.at(gameObjectIndex)->getComponents<RigidBody>();

        if (!rigidBodies.empty()) {
            if (rigidBodies[0]->getBodyId() == -1) {

                BodyProxy bodyProxy = BodyProxy(mGameObjects.at(gameObjectIndex));

                int bodyID = mWorld.createBody(bodyProxy);
                rigidBodies[0]->setBodyId(bodyID);

                for (BoxCollider* boxCollider : bodyProxy.getBoxColliders()) {
                    boxCollider->setWidth(boxCollider->getWidth() * 2);
                    boxCollider->setHeight(boxCollider->getHeight() * 2);
                }
            }
        }
    }
}

void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld.createWorld(aGravity); }

World& PhysicsEngine::getWorld() { return mWorld; }

void PhysicsEngine::setgameObjects(std::vector<GameObject*> aGameObjects) { mGameObjects = aGameObjects; }

void PhysicsEngine::reset() {
    mWorld.resetWorld();
    for (int gameObjectIndex = 0; gameObjectIndex < mGameObjects.size(); gameObjectIndex++) {
        std::vector<RigidBody*> rigidBodies = mGameObjects.at(gameObjectIndex)->getComponents<RigidBody>();
        for (int rigidBodyIndex = 0; rigidBodyIndex < rigidBodies.size(); rigidBodyIndex++) {
            rigidBodies.at(rigidBodyIndex)->setBodyId(-1);
        }
    }
}

GameObject* PhysicsEngine::getGameObjectByID(int aID) {
    for (int i = 0; i < mGameObjects.size(); i++) {
        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();
        if (!rigidBodies.empty()) {
            if (rigidBodies[0]->getBodyId() == aID) {
                return mGameObjects.at(i);
            }
        }
    }
    return nullptr;
}
