#include "Physics/PhysicsEngine.h"
#include "GameObject.h"
#include "Input.h"
#include "Physics/BodyProxy.h"
#include <vector>

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::updateReferences(std::vector<GameObject*>& aGameObjects) { mGameObjects = aGameObjects; }

void PhysicsEngine::update() {
    // Input& input = Input::getInstance();

    for (int i = 0; i < mGameObjects.size(); i++) {

        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();

        if (!rigidBodies.empty()) {
            BodyProxy bodyProxy = BodyProxy(mGameObjects.at(i));
            int id = rigidBodies[0]->getBodyId();

            mWorld.updateBody(id, bodyProxy);
        }
    }
    // if (input.GetKeyDown(Key::Key_Space)) {
    //     mWorld.applyForce(1, Vector2(10000, 10000));
    //     std::cout << "Force applied" << std::endl;
    // }
    // Simulate the world for a few steps
    float timeStep = 30.0f / 60.0f;
    int velocityIterations = 20;
    int positionIterations = 2;
    mWorld.executeWorldStep(timeStep, velocityIterations);

    mWorld.getContactEvents();
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

                std::cout << "Position: " << position.x << " " << position.y << std::endl;
            }
        }
    }
}

void PhysicsEngine::setSubStep(int aSubStep) {}

float PhysicsEngine::getSubStep() const { return mSubStep; }

void PhysicsEngine::setStep(float aStep) {}

float PhysicsEngine::getStep() const { return mStep; }

void PhysicsEngine::executeCollisionScripts(std::vector<int> aBodyIDs) {}

void PhysicsEngine::createBodies() {
    std::cout << "PhysicsEngine::createBodies()" << std::endl;
    std::cout << "Number of game objects: " << mGameObjects.size() << std::endl;
    for (int i = 0; i < mGameObjects.size(); i++) {
        std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();

        BodyProxy bodyProxy = BodyProxy(mGameObjects.at(i));

        if (bodyProxy.getvalidBody()) {
            int bodyID = mWorld.createBody(bodyProxy);
            rigidBodies[0]->setBodyId(bodyID);
        }

        for (BoxCollider* boxCollider : bodyProxy.getBoxColliders()) {
            // std::cout << "BoxCollider width: " << boxCollider->getWidth() << std::endl;
            // std::cout << "BoxCollider height: " << boxCollider->getHeight() << std::endl;
            boxCollider->setWidth(boxCollider->getWidth() * 2);
            boxCollider->setHeight(boxCollider->getHeight() * 2);
        }
    }
}

void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld.createWorld(aGravity); }

World& PhysicsEngine::getWorld() { return mWorld; }

void PhysicsEngine::setgameObjects(std::vector<GameObject*> aGameObjects) { mGameObjects = aGameObjects; }
