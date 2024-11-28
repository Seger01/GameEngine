#include "Animation.h"
#include "BodyProxy.h"
#include "BoxCollider.h"
#include "Camera.h"
#include "CircleCollider.h"
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
#include <functional>
#include <gtest/gtest.h>

class PhysicsTest : public ::testing::Test {
protected:
    PhysicsEngine* mPhysicsEngine;

    void SetUp() override {
        mPhysicsEngine = new PhysicsEngine();
        mPhysicsEngine->createWorld(Vector2(0, 0));
    }
    void TearDown() override { delete mPhysicsEngine; }
};

TEST_F(PhysicsTest, WorldStep) {
    ASSERT_NO_THROW(mPhysicsEngine->setSubStep(4));
    ASSERT_NO_THROW(mPhysicsEngine->setStep(1.0f / 60.0f));

    ASSERT_EQ(mPhysicsEngine->getSubStep(), 4);
    ASSERT_EQ(mPhysicsEngine->getStep(), 1.0f / 60.0f);

    mPhysicsEngine->getWorld().setGravity(Vector2(0, 0));
    ASSERT_EQ(mPhysicsEngine->getWorld().getGravity(), Vector2(0, 0));
}

TEST_F(PhysicsTest, WorldDestroy) { mPhysicsEngine->reset(); }

TEST_F(PhysicsTest, BodyProxy) {
    std::vector<std::reference_wrapper<GameObject>> gameObjects;
    GameObject* gameObject = new GameObject();

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
    gameObjects.push_back(*gameObject);
    BodyProxy* proxy = new BodyProxy(*gameObject);

    ASSERT_EQ(proxy->getHasGravity(), false);
    ASSERT_EQ(proxy->getCanRotate(), false);
    ASSERT_EQ(proxy->getIsMoveableByForce(), true);
    ASSERT_EQ(proxy->getDensity(), 1.0f);
    ASSERT_EQ(proxy->getFriction(), 0.6f);
    ASSERT_EQ(proxy->getRestitution(), 0.0f);
    ASSERT_EQ(proxy->getGravityScale(), 0.0f);
    ASSERT_EQ(proxy->getMass(), 5.0f);
    ASSERT_EQ(proxy->getLinearDamping(), 0.5f);
    ASSERT_EQ(proxy->getAngularDamping(), 0.5f);
    ASSERT_NO_THROW(proxy->getBoxColliders());
    ASSERT_NO_THROW(proxy->getCircleColliders());

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
    //// mPhysicsEngine->convertToBox2D(gameObject);

    BodyProxy proxy = BodyProxy(*gameObject);

    mPhysicsEngine->getWorld().createBody(proxy);

    ASSERT_NO_THROW(mPhysicsEngine->createBodies());
    ASSERT_NO_THROW(mPhysicsEngine->createBodies());
    mPhysicsEngine->reset();
}

TEST_F(PhysicsTest, updateloop) {
    PhysicsManager* physicsManager = new PhysicsManager();
    physicsManager->startPhysicsEngine({0.0f, 0.0f});

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
    gameObject->getComponents<RigidBody>().at(0)->addTorque(10);

    Transform transform2;
    transform2.position = {8, 0.0f};
    gameObject2->addComponent<BoxCollider>();

    BoxCollider* boxCollider2 = gameObject2->getComponents<BoxCollider>()[0];
    boxCollider2->setWidth(8);
    boxCollider2->setHeight(25);
    boxCollider2->setTransform(transform2);
    boxCollider2->setTrigger(false);

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

    gameObjects.push_back(gameObject);
    gameObjects.push_back(gameObject2);

    gameObjects.at(1)->getComponents<BoxCollider>().at(0)->setTrigger(true);

    ASSERT_NO_THROW(mPhysicsEngine->update());
}

TEST_F(PhysicsTest, testcollide) {

    std::vector<GameObject*> gameObjects;

    GameObject* gameObject = new GameObject();

    Transform objectTransform;
    objectTransform.position.x = 100;
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
    gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));
    gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));
    gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));

    gameObjects.push_back(gameObject);

    Transform transform2;
    transform2.position = {0.0f, 0.0f};
    Transform objectTransform2;
    objectTransform2.position.x = 100;
    objectTransform2.position.y = 120;

    gameObject->setTransform(objectTransform);

    GameObject* gameObject2 = new GameObject();

    gameObject2->addComponent<BoxCollider>();

    BoxCollider* boxCollider2 = gameObject2->getComponents<BoxCollider>()[0];
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
    rigidBody2->setActive(false);

    gameObject2->addComponent(rigidBody2);

    mPhysicsEngine->addObject(*gameObject);
    mPhysicsEngine->addObject(*gameObject2);

    ASSERT_NO_THROW(mPhysicsEngine->update());

    std::vector<std::pair<int, int>> bodyIDs;
    std::pair<int, int> collide;
    collide.first = 1;
    collide.second = 2;

    bodyIDs.push_back(collide);

    mPhysicsEngine->executeCollisionScripts(bodyIDs);
}

TEST_F(PhysicsTest, BoxCollider) {
    BoxCollider* boxCollider = new BoxCollider;

    boxCollider->setWidth(10);
    ASSERT_EQ(boxCollider->getWidth(), 10);

    boxCollider->setHeight(10);
    ASSERT_EQ(boxCollider->getHeight(), 10);

    boxCollider->setRotation(10);
    ASSERT_EQ(boxCollider->getRotation(), 10);

    boxCollider->setTrigger(true);
    ASSERT_TRUE(boxCollider->isTrigger());

    ASSERT_NO_THROW(boxCollider->clone());
}

TEST_F(PhysicsTest, CircleCollider) {
    CircleCollider* circleCollider = new CircleCollider(10);
    CircleCollider* circleCollider2 = new CircleCollider();

    circleCollider->setRadius(20);
    ASSERT_EQ(circleCollider->getRadius(), 20);

    ASSERT_NO_THROW(circleCollider->clone());
}

TEST_F(PhysicsTest, properties) {

    std::vector<GameObject*> gameObjects;

    GameObject* gameObject = new GameObject();

    Transform objectTransform;
    objectTransform.position.x = 100;
    objectTransform.position.y = 100;

    gameObject->setTransform(objectTransform);

    RigidBody* rigidBody = new RigidBody();

    rigidBody->setCanRotate(false);
    rigidBody->setHasGravity(false);
    rigidBody->setIsMoveableByForce(false);
    rigidBody->setDensity(0.0);
    rigidBody->setFriction(0.0f);
    rigidBody->setRestitution(0.0f);
    rigidBody->setGravityScale(0.0f);
    rigidBody->setMass(0.0f);
    rigidBody->setLinearDamping(0.0f);
    rigidBody->setAngularDamping(0.0f);
    rigidBody->setActive(false);

    gameObject->addComponent(rigidBody);

    Transform transform;
    transform.position = {0.0f, 0.0f};
    gameObject->addComponent<BoxCollider>();
    BoxCollider* boxCollider = gameObject->getComponents<BoxCollider>()[0];
    boxCollider->setTrigger(false);
    boxCollider->setWidth(8);   // 16
    boxCollider->setHeight(25); // 25
    boxCollider->setTransform(transform);
    boxCollider->setTrigger(true);

    gameObject->setTransform(objectTransform);

    mPhysicsEngine->addObject(*gameObject);

    ASSERT_NO_THROW(mPhysicsEngine->update());
    rigidBody->setActive(true);
    boxCollider->setTrigger(false);
    ASSERT_EQ(boxCollider->isTrigger(), false);

    ASSERT_NO_THROW(mPhysicsEngine->update());
}

TEST_F(PhysicsTest, remove) {

    std::vector<GameObject*> gameObjects;

    GameObject* gameObject = new GameObject();

    Transform objectTransform;
    objectTransform.position.x = 100;
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
    gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));
    gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));
    gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));

    gameObjects.push_back(gameObject);

    Transform transform2;
    transform2.position = {0.0f, 0.0f};
    Transform objectTransform2;
    objectTransform2.position.x = 100;
    objectTransform2.position.y = 120;

    gameObject->setTransform(objectTransform);

    GameObject* gameObject2 = new GameObject();

    gameObject2->addComponent<BoxCollider>();

    BoxCollider* boxCollider2 = gameObject2->getComponents<BoxCollider>()[0];
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
    rigidBody2->setActive(false);

    gameObject2->addComponent(rigidBody2);

    mPhysicsEngine->addObject(*gameObject);
    mPhysicsEngine->addObject(*gameObject2);

    ASSERT_NO_THROW(mPhysicsEngine->update());

    std::vector<std::pair<int, int>> bodyIDs;
    std::pair<int, int> collide;
    collide.first = 1;
    collide.second = 2;

    bodyIDs.push_back(collide);

    mPhysicsEngine->executeCollisionScripts(bodyIDs);

    mPhysicsEngine->removeObject(*gameObject);
    mPhysicsEngine->removeObject(*gameObject2);

    mPhysicsEngine->executeCollisionScripts(bodyIDs);
}
