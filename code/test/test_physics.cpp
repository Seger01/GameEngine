#include "Animation.h"
#include "BodyProxy.h"
#include "Camera.h"
#include "Color.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "PhysicsManager.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sprite.h"
#include "SpriteDefUtil.h"
#include "Text.h"
#include "Vector2.h"
#include "Window.h"
#include <gtest/gtest.h>

class PhysicsTest : public ::testing::Test {
protected:
    // You can initialize common resources here
    PhysicsEngine* mPhysicsEngine;

    void SetUp() override {
        // Initialize the RenderSystem and necessary components
        mPhysicsEngine = new PhysicsEngine();
        mPhysicsEngine->createWorld(Vector2(0, 0));
    }
    void TearDown() override {
        // Clean up
        delete mPhysicsEngine;
    }
};

// TEST_F(PhysicsTest, WorldCreate) {
//     // Create a new world
//     ASSERT_NO_THROW(mPhysicsEngine->createWorld(Vector2(0, 0)));
//     ASSERT_EQ(mPhysicsEngine->getWorld().getGravity(), Vector2(0, 0));
//     mPhysicsEngine->reset();
// }

TEST_F(PhysicsTest, WorldStep) {
    // Create a new world
    // Step the world
    // mPhysicsEngine->createWorld(Vector2(0, 0));
    ASSERT_NO_THROW(mPhysicsEngine->setSubStep(4));
    ASSERT_NO_THROW(mPhysicsEngine->setStep(1.0f / 60.0f));

    ASSERT_EQ(mPhysicsEngine->getSubStep(), 4);
    ASSERT_EQ(mPhysicsEngine->getStep(), 1.0f / 60.0f);
    //  mPhysicsEngine->reset();
}

TEST_F(PhysicsTest, WorldDestroy) {
    // Create a new world
    // Destroy the world
    mPhysicsEngine->reset();

    // ASSERT_NO_THROW(mPhysicsEngine->getWorld().resetWorld());
}

TEST_F(PhysicsTest, BodyProxy) {
    std::vector<GameObject*> gameObjects;
    GameObject* gameObject = new GameObject();
    GameObject* gameObject2 = new GameObject();

    RigidBody* rigidBody = new RigidBody();

    rigidBody->setCanRotate(false);
    rigidBody->setHasGravity(false);
    rigidBody->setIsMoveableByForce(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.6f);
    rigidBody->setRestitution(0.0f);
    rigidBody->setGravityScale(0.0f);
    rigidBody->setMass(5.0f);
    rigidBody->setLinearDamping(0.5f);
    rigidBody->setAngularDamping(0.5f);

    gameObject->addComponent(rigidBody);

    Transform transform;
    transform.position = {0.0f, 0.0f};
    BoxCollider* boxCollider = new BoxCollider();
    boxCollider->setWidth(16);  // 16
    boxCollider->setHeight(25); // 25
    boxCollider->setTransform(transform);

    gameObject->addComponent(boxCollider);
    gameObjects.push_back(gameObject);

    ASSERT_NO_THROW(BodyProxy* proxy = new BodyProxy(gameObject));
    mPhysicsEngine->reset();
}

TEST_F(PhysicsTest, CreateBodiesWithBody) {

    std::vector<GameObject*> gameObjects;

    GameObject* gameObject = new GameObject();
    GameObject* gameObject2 = new GameObject();

    Transform objectTransform;
    objectTransform.position.x = 80;
    objectTransform.position.y = 100;

    gameObject->setTransform(objectTransform);

    RigidBody* rigidBody = new RigidBody();

    rigidBody->setCanRotate(false);
    rigidBody->setHasGravity(false);
    rigidBody->setIsMoveableByForce(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.6f);
    rigidBody->setRestitution(0.0f);
    rigidBody->setGravityScale(0.0f);
    rigidBody->setMass(5.0f);
    rigidBody->setLinearDamping(0.5f);
    rigidBody->setAngularDamping(0.5f);

    gameObject->addComponent(rigidBody);

    Transform transform;
    transform.position = {0.0f, 0.0f};
    gameObject->addComponent<BoxCollider>();
    BoxCollider* boxCollider = gameObject->getComponents<BoxCollider>()[0];
    boxCollider->setWidth(8);   // 16
    boxCollider->setHeight(25); // 25
    boxCollider->setTransform(transform);

    Transform transform2;
    transform2.position = {8, 0.0f};

    BoxCollider* boxCollider2 = new BoxCollider();
    boxCollider2->setWidth(8);
    boxCollider2->setHeight(25);
    boxCollider2->setTransform(transform2);

    gameObject->addComponent(boxCollider);
    gameObject->addComponent(boxCollider2);
    gameObjects.push_back(gameObject);
    gameObjects.push_back(gameObject2);

    gameObject->addComponent(boxCollider);
    mPhysicsEngine->convertToBox2D(gameObject);

    BodyProxy proxy = BodyProxy(gameObject);

    mPhysicsEngine->getWorld().createBody(proxy);

    ASSERT_NO_THROW(mPhysicsEngine->updateReferences(gameObjects));

    ASSERT_NO_THROW(mPhysicsEngine->createBodies());
    ASSERT_NO_THROW(mPhysicsEngine->createBodies());
    mPhysicsEngine->reset();
}

// TEST_F(PhysicsTest, CreateBodiesWithoutBody) {
//     mPhysicsEngine->createWorld(Vector2(0, 0));
//
//     ASSERT_NO_THROW(mPhysicsEngine->createBodies());
// }

TEST_F(PhysicsTest, updateloop) {

    std::vector<GameObject*> gameObjects;

    GameObject* gameObject = new GameObject();
    GameObject* gameObject2 = new GameObject();

    Transform objectTransform;
    objectTransform.position.x = 80;
    objectTransform.position.y = 100;

    gameObject->setTransform(objectTransform);

    RigidBody* rigidBody = new RigidBody();

    rigidBody->setCanRotate(false);
    rigidBody->setHasGravity(false);
    rigidBody->setIsMoveableByForce(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.6f);
    rigidBody->setRestitution(0.0f);
    rigidBody->setGravityScale(0.0f);
    rigidBody->setMass(5.0f);
    rigidBody->setLinearDamping(0.5f);
    rigidBody->setAngularDamping(0.5f);

    gameObject->addComponent(rigidBody);

    Transform transform;
    transform.position = {0.0f, 0.0f};
    gameObject->addComponent<BoxCollider>();
    BoxCollider* boxCollider = gameObject->getComponents<BoxCollider>()[0];
    boxCollider->setWidth(8);   // 16
    boxCollider->setHeight(25); // 25
    boxCollider->setTransform(transform);
    gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(200, 0));

    Transform transform2;
    transform2.position = {8, 0.0f};

    BoxCollider* boxCollider2 = new BoxCollider();
    boxCollider2->setWidth(8);
    boxCollider2->setHeight(25);
    boxCollider2->setTransform(transform2);

    RigidBody* rigidBody2 = new RigidBody();

    rigidBody2->setCanRotate(false);
    rigidBody2->setHasGravity(false);
    rigidBody2->setIsMoveableByForce(true);
    rigidBody2->setDensity(1.0f);
    rigidBody2->setFriction(0.6f);
    rigidBody2->setRestitution(0.0f);
    rigidBody2->setGravityScale(0.0f);
    rigidBody2->setMass(5.0f);
    rigidBody2->setLinearDamping(0.5f);
    rigidBody2->setAngularDamping(0.5f);

    gameObject2->addComponent(rigidBody2);

    gameObject->addComponent(boxCollider);
    gameObject->addComponent(boxCollider2);
    gameObjects.push_back(gameObject);
    gameObjects.push_back(gameObject2);

    mPhysicsEngine->updateReferences(gameObjects);

    ASSERT_NO_THROW(mPhysicsEngine->update());
}

// TEST_F(PhysicsTest, collision) {
//
//     std::vector<GameObject*> gameObjects;
//
//     GameObject* gameObject = new GameObject();
//     GameObject* gameObject2 = new GameObject();
//
//     Transform objectTransform;
//     objectTransform.position.x = 80;
//     objectTransform.position.y = 100;
//
//     gameObject->setTransform(objectTransform);
//
//     RigidBody* rigidBody = new RigidBody();
//
//     rigidBody->setCanRotate(false);
//     rigidBody->setHasGravity(false);
//     rigidBody->setIsMoveableByForce(true);
//     rigidBody->setDensity(1.0f);
//     rigidBody->setFriction(0.6f);
//     rigidBody->setRestitution(0.0f);
//     rigidBody->setGravityScale(0.0f);
//     rigidBody->setMass(5.0f);
//     rigidBody->setLinearDamping(0.5f);
//     rigidBody->setAngularDamping(0.5f);
//
//     gameObject->addComponent(rigidBody);
//
//     Transform transform;
//     transform.position = {0.0f, 0.0f};
//     gameObject->addComponent<BoxCollider>();
//     BoxCollider* boxCollider = gameObject->getComponents<BoxCollider>()[0];
//     boxCollider->setWidth(8);   // 16
//     boxCollider->setHeight(25); // 25
//     boxCollider->setTransform(transform);
//
//     Transform objectTransform2;
//     objectTransform.position.x = 50;
//     objectTransform.position.y = 100;
//
//     gameObject2->setTransform(objectTransform2);
//
//     Transform transform2;
//     transform2.position = {8, 0.0f};
//
//     RigidBody* rigidBody2 = new RigidBody();
//
//     rigidBody2->setCanRotate(false);
//     rigidBody2->setHasGravity(false);
//     rigidBody2->setIsMoveableByForce(true);
//     rigidBody2->setDensity(1.0f);
//     rigidBody2->setFriction(0.6f);
//     rigidBody2->setRestitution(0.0f);
//     rigidBody2->setGravityScale(0.0f);
//     rigidBody2->setMass(5.0f);
//     rigidBody2->setLinearDamping(0.5f);
//     rigidBody2->setAngularDamping(0.5f);
//
//     gameObject2->addComponent(rigidBody2);
//
//     BoxCollider* boxCollider2 = new BoxCollider();
//     boxCollider2->setWidth(8);
//     boxCollider2->setHeight(25);
//     boxCollider2->setTransform(transform2);
//
//     gameObject->addComponent(boxCollider);
//     gameObject->addComponent(boxCollider2);
//
//     gameObjects.push_back(gameObject);
//     gameObjects.push_back(gameObject2);
//
//     // ASSERT_NO_THROW(mPhysicsEngine->setSubStep(4));
//     // ASSERT_NO_THROW(mPhysicsEngine->setStep(1.0f / 60.0f));
//
//     mPhysicsEngine->updateReferences(gameObjects);
//
//     ASSERT_NO_THROW(mPhysicsEngine->update());
// }
