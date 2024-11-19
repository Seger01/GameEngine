#include "Physics/PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::updateReferences(std::vector<GameObject*>& aGameObjects) { mGameObjects = aGameObjects; }

void PhysicsEngine::update() {
    createBodies();
    updateFlags();

    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {

            RigidBody* rigidBody = mGameObjects.at(i)->getComponents<RigidBody>()[0];

            Transform transform = mGameObjects.at(i)->getTransform();

            transform.position =
                transform.position + mGameObjects.at(i)->getComponents<BoxCollider>()[0]->getTransform().position;

            transform.position.x =
                transform.position.x + mGameObjects.at(i)->getComponents<BoxCollider>()[0]->getWidth();
            transform.position.y =
                transform.position.y + mGameObjects.at(i)->getComponents<BoxCollider>()[0]->getHeight();

            Vector2 newPos = Vector2(-transform.position.x, -transform.position.y);

            // Vector2 readPos = mWorld.getPosition(rigidBody->getBodyId());
            // std::cout << "-------------------" << std::endl;
            // std::cout << "ReadPos: " << readPos.x << " " << readPos.y << std::endl;
            // std::cout << "NewPos: " << newPos.x << " " << newPos.y << std::endl;
            // std::cout << "Collider Size: " << mGameObjects.at(i)->getComponents<BoxCollider>()[0]->getWidth() << " "
            //           << mGameObjects.at(i)->getComponents<BoxCollider>()[0]->getHeight() << std::endl;
            // std::cout << "____________________" << std::endl;
            mWorld.setPosition(rigidBody->getBodyId(), newPos);
            // BodyProxy bodyProxy = BodyProxy(mGameObjects.at(i));
            // mWorld.updateBody(rigidBody->getBodyId(), bodyProxy);

            // for (BoxCollider* boxCollider : bodyProxy.getBoxColliders()) {
            //     boxCollider->setWidth(boxCollider->getWidth() * 2);
            //     boxCollider->setHeight(boxCollider->getHeight() * 2);
            // }

            // if (mGameObjects.at(i)->hasComponent<BoxCollider>()) {
            //     BoxCollider* boxCollider = mGameObjects.at(i)->getComponents<BoxCollider>()[0];
            //
            //     boxCollider->setWidth(boxCollider->getWidth() * 2);
            //     boxCollider->setHeight(boxCollider->getHeight() * 2);
            //
            //     // Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
            //     // Transform transform = mGameObjects.at(i)->getTransform();
            //     //
            //     // transform.position = position;
            //     //
            //     // transform = Transform(Vector2(-position.x, -position.y));
            //     //
            //     // transform.position.x = transform.position.x + (boxCollider->getWidth() / 2);
            //     // transform.position.y = transform.position.y + (boxCollider->getHeight() / 2);
            //     //
            //     // transform.position = transform.position - boxCollider->getTransform().position;
            //
            //     // mGameObjects.at(i)->setTransform(transform);
            // }
        }
    }

    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {

            RigidBody* rigidBody = mGameObjects.at(i)->getComponents<RigidBody>()[0];

            mWorld.applyLinearForce(rigidBody->getBodyId(), rigidBody->getForcesBuffer());
            rigidBody->clearForcesBuffer();
        }
    }

    float timeStep = 20.0f / 60.0f;
    int velocityIterations = 4;
    int positionIterations = 2;
    mWorld.executeWorldStep(timeStep, velocityIterations);

    executeCollisionScripts(mWorld.getContactEvents());
    executeCollisionScripts(mWorld.getSensorEvents());

    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {
            RigidBody* rigidBody = mGameObjects.at(i)->getComponents<RigidBody>()[0];

            if (mGameObjects.at(i)->hasComponent<BoxCollider>()) {
                BoxCollider* boxCollider = mGameObjects.at(i)->getComponents<BoxCollider>()[0];

                boxCollider->setWidth(boxCollider->getWidth() * 2);
                boxCollider->setHeight(boxCollider->getHeight() * 2);

                Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
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

        if (mGameObjects.at(gameObjectIndex)->hasComponent<RigidBody>()) {
            RigidBody* rigidBody = mGameObjects.at(gameObjectIndex)->getComponents<RigidBody>()[0];

            if (rigidBody->getBodyId() == -1) {

                BodyProxy bodyProxy = BodyProxy(mGameObjects.at(gameObjectIndex));

                int bodyID = mWorld.createBody(bodyProxy);
                rigidBody->setBodyId(bodyID);

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

// Checks and returns GameObject if BodyID exists within world
GameObject* PhysicsEngine::getGameObjectByID(int aID) {
    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {
            std::vector<RigidBody*> rigidBodies = mGameObjects.at(i)->getComponents<RigidBody>();
            if (!rigidBodies.empty()) {
                if (rigidBodies[0]->getBodyId() == aID) {
                    return mGameObjects.at(i);
                }
            }
        }
    }
    return nullptr;
}

void PhysicsEngine::setCollision(int aBodyID, bool aState) {
    std::cout << "removed function: setCollision, check function for new method" << std::endl;
    // setting of collision is now handled by bool isactive from mGameObjects upon update
}

void PhysicsEngine::updateFlags() {
    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects.at(i)->hasComponent<RigidBody>()) {
            int bodyID = mGameObjects.at(i)->getComponents<RigidBody>()[0]->getBodyId();
            BodyProxy bodyProxy = BodyProxy(mGameObjects.at(i));
            mWorld.updateBodyFlags(bodyProxy, bodyID);

            mWorld.setBodyActivity(bodyID, mGameObjects.at(i)->getComponents<RigidBody>().at(0)->isActive());
        }
    }
}

GameObject* PhysicsEngine::convertFromBox2D(GameObject* aGameObject) {
    if (aGameObject->hasComponent<RigidBody>()) {
        RigidBody* rigidBody = aGameObject->getComponents<RigidBody>()[0];

        if (aGameObject->hasComponent<BoxCollider>()) {
            BoxCollider* boxCollider = aGameObject->getComponents<BoxCollider>()[0];

            boxCollider->setWidth(boxCollider->getWidth() * 2);
            boxCollider->setHeight(boxCollider->getHeight() * 2);

            Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
            Transform transform = aGameObject->getTransform();

            transform.position = position;

            transform = Transform(Vector2(-position.x, -position.y));

            transform.position.x = transform.position.x - boxCollider->getWidth() / 2;
            transform.position.y = transform.position.y - boxCollider->getHeight() / 2;

            transform.position = transform.position - boxCollider->getTransform().position;

            aGameObject->setTransform(transform);
        }
    }
    return aGameObject;
}

GameObject* PhysicsEngine::convertToBox2D(GameObject* aGameObject) {
    std::vector<BoxCollider*> boxColliders = aGameObject->getComponents<BoxCollider>();

    for (BoxCollider* boxCollider : boxColliders) {
        boxCollider->setWidth(boxCollider->getWidth() / 2);
        boxCollider->setHeight(boxCollider->getHeight() / 2);
    }

    Transform transform = aGameObject->getTransform();

    transform.position = transform.position + boxColliders[0]->getTransform().position;

    transform.position.x = transform.position.x + boxColliders.at(0)->getWidth() / 2;
    transform.position.y = transform.position.y + boxColliders.at(0)->getHeight() / 2;

    transform.position = Vector2(-transform.position.x, -transform.position.y);

    return aGameObject;
}
