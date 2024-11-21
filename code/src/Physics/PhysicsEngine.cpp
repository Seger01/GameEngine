#include "Physics/PhysicsEngine.h"
#include "GameObject.h"
#include "RigidBody.h"

PhysicsEngine::PhysicsEngine() {}

PhysicsEngine::~PhysicsEngine() {}
void PhysicsEngine::updateReferences(std::vector<GameObject*>& aGameObjects) { mGameObjects = aGameObjects; }

void PhysicsEngine::update() {
    for (GameObject* gameObject : mGameObjects) {
        if (gameObject->hasComponent<RigidBody>()) {
            convertToBox2D(gameObject);
        }
    }
    createBodies();
    updateFlags();

    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {

            RigidBody* rigidBody = mGameObjects.at(i)->getComponents<RigidBody>()[0];

            Transform transform = mGameObjects.at(i)->getTransform();

            transform.position.x =
                transform.position.x - mGameObjects.at(i)->getComponents<BoxCollider>()[0]->getWidth();
            transform.position.y =
                transform.position.y - mGameObjects.at(i)->getComponents<BoxCollider>()[0]->getHeight();

            Vector2 newPos = Vector2(transform.position.x, transform.position.y);

            mWorld.setPosition(newPos, rigidBody->getBodyId());
        }
    }

    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {

            RigidBody* rigidBody = mGameObjects.at(i)->getComponents<RigidBody>()[0];

            mWorld.applyLinearForce(rigidBody->getForcesBuffer(), rigidBody->getBodyId());
            rigidBody->clearForcesBuffer();
        }
    }

    float timeStep = 20.0f / 60.0f;
    int velocityIterations = 4;
    int positionIterations = 2;
    mWorld.executeWorldStep(timeStep, velocityIterations);

    executeCollisionScripts(mWorld.getContactEvents());
    executeCollisionScripts(mWorld.getSensorEvents());
    for (GameObject* gameObject : mGameObjects) {
        if (gameObject->hasComponent<RigidBody>()) {
            convertFromBox2D(gameObject);
        }
    }
}

void PhysicsEngine::setSubStep(int aSubStep) { mSubStep = aSubStep; }

float PhysicsEngine::getSubStep() const { return mSubStep; }

void PhysicsEngine::setStep(float aStep) { mStep = aStep; }

float PhysicsEngine::getStep() const { return mStep; }

void PhysicsEngine::executeCollisionScripts(std::vector<std::pair<int, int>> aBodyIDs) {

    for (int i = 0; i < aBodyIDs.size(); i++) {
        GameObject* gameObjectA = getGameObjectByID(aBodyIDs.at(i).first);
        GameObject* gameObjectB = getGameObjectByID(aBodyIDs.at(i).second);

        if (gameObjectA != nullptr) {

            if (gameObjectA->hasComponent<IBehaviourScript>()) {
                std::vector<IBehaviourScript*> behaviourScript = gameObjectA->getComponents<IBehaviourScript>();

                for (int i = 0; i < behaviourScript.size(); i++) {
                    behaviourScript.at(i)->onCollide(gameObjectB);
                }
            }
        }

        if (gameObjectB != nullptr) {

            if (gameObjectB->hasComponent<IBehaviourScript>()) {
                if (gameObjectB->hasComponent<IBehaviourScript>()) {
                    std::vector<IBehaviourScript*> behaviourScript = gameObjectB->getComponents<IBehaviourScript>();

                    for (int i = 0; i < behaviourScript.size(); i++) {
                        behaviourScript.at(i)->onCollide(gameObjectA);
                    }
                }
            }
        }
    }
}

void PhysicsEngine::createBodies() {
    for (int gameObjectIndex = 0; gameObjectIndex < mGameObjects.size(); gameObjectIndex++) {
        std::cout << "Creating body for game object " << gameObjectIndex << std::endl;
        if (mGameObjects.at(gameObjectIndex)->hasComponent<RigidBody>()) {
            RigidBody* rigidBody = mGameObjects.at(gameObjectIndex)->getComponents<RigidBody>()[0];

            if (rigidBody->getBodyId().bodyID == -1) {

                BodyProxy bodyProxy = BodyProxy(mGameObjects.at(gameObjectIndex));

                BodyID bodyID = mWorld.createBody(bodyProxy);
                rigidBody->setBodyId(bodyID);
            }
        }
    }
}

void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld = World(aGravity); }

World& PhysicsEngine::getWorld() { return mWorld; }

void PhysicsEngine::reset() {
    mWorld.resetWorld();
    // for (int gameObjectIndex = 0; gameObjectIndex < mGameObjects.size(); gameObjectIndex++) {
    //     std::vector<RigidBody*> rigidBodies = mGameObjects.at(gameObjectIndex)->getComponents<RigidBody>();
    //     for (int rigidBodyIndex = 0; rigidBodyIndex < rigidBodies.size(); rigidBodyIndex++) {
    //         rigidBodies.at(rigidBodyIndex)->setBodyId({-1, 0});
    //     }
    // }
}

// Checks and returns GameObject if BodyID exists within world
GameObject* PhysicsEngine::getGameObjectByID(int aID) {
    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {
            std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();
            if (!rigidBodies.empty()) {
                if (rigidBodies[0]->getBodyId().bodyID == aID) {
                    return mGameObjects.at(i);
                }
            }
        }
    }
    return nullptr;
}

void PhysicsEngine::updateFlags() {
    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {
            RigidBody* body = mGameObjects.at(i)->getComponents<RigidBody>()[0];
            BodyID bodyID = mGameObjects.at(i)->getComponents<RigidBody>()[0]->getBodyId();

            if (body->getIsUpdated()) {
                BodyProxy bodyProxy = BodyProxy(mGameObjects.at(i));
                mWorld.updateBodyProperties(bodyProxy, bodyID);
                mWorld.updateShapeProperties(bodyProxy, bodyID);

                body->setIsUpdated(false);
            }
            mWorld.setBodyActivity(mGameObjects.at(i)->getComponents<RigidBody>().at(0)->isActive(), bodyID);
        }
    }
}

void PhysicsEngine::convertFromBox2D(GameObject* aGameObject) {
    RigidBody* rigidBody = aGameObject->getComponents<RigidBody>()[0];

    if (aGameObject->hasComponent<BoxCollider>()) {
        BoxCollider* boxCollider = aGameObject->getComponents<BoxCollider>()[0];

        Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
        Transform transform = aGameObject->getTransform();

        transform.position = position;

        transform = Transform(Vector2(position.x, position.y));

        transform.position.x = transform.position.x - boxCollider->getWidth();
        transform.position.y = transform.position.y - boxCollider->getHeight();

        for (BoxCollider* boxCollider : aGameObject->getComponents<BoxCollider>()) {
            boxCollider->setWidth(boxCollider->getWidth() * 2);
            boxCollider->setHeight(boxCollider->getHeight() * 2);
        }

        aGameObject->setTransform(transform);
    }
}

void PhysicsEngine::convertToBox2D(GameObject* aGameObject) {

    Transform transform = aGameObject->getTransform();
    std::vector<BoxCollider*> mBoxColliders = aGameObject->getComponents<BoxCollider>();

    transform.position.x = transform.position.x + mBoxColliders.at(0)->getWidth();
    transform.position.y = transform.position.y + mBoxColliders.at(0)->getHeight();
    for (BoxCollider* boxCollider : mBoxColliders) {
        boxCollider->setWidth(boxCollider->getWidth() / 2);
        boxCollider->setHeight(boxCollider->getHeight() / 2);
    }

    aGameObject->setTransform(Vector2(transform.position.x, transform.position.y));
}
