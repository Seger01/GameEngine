#include "BodyID.h"
#include "BodyProxy.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "PhysicsEngine.h"
#include "PhysicsManager.h"
#include "RigidBody.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include <functional>
#include <gtest/gtest.h>

class PhysicsTest : public ::testing::Test
{
protected:
	PhysicsManager* mPhysicsManager;
	PhysicsEngine mPhysicsEngine;

	void SetUp() override
	{
		mPhysicsManager = new PhysicsManager();
		mPhysicsManager->startPhysicsEngine({0.0f, 0.0f});
		mPhysicsEngine = mPhysicsManager->getPhysicsEngine();
	}

	void TearDown() override { delete mPhysicsManager; }
};

TEST_F(PhysicsTest, WorldStep)
{
	ASSERT_NO_THROW(mPhysicsManager->getPhysicsEngine().setSubStep(4));
	ASSERT_NO_THROW(mPhysicsEngine.setStep(1.0f / 60.0f));

	ASSERT_EQ(mPhysicsEngine.getSubStep(), 4);
	ASSERT_EQ(mPhysicsEngine.getStep(), 1.0f / 60.0f);

	mPhysicsEngine.getWorld().setGravity(Vector2(0, 0));
	ASSERT_EQ(mPhysicsEngine.getWorld().getGravity(), Vector2(0, 0));
}

TEST_F(PhysicsTest, BodyProxy)
{
	std::vector<std::reference_wrapper<GameObject>> gameObjects;
	GameObject* gameObject = new GameObject();
	gameObject->addComponent<RigidBody>();
	RigidBody* rigidBody = gameObject->getComponents<RigidBody>()[0];

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

	Transform transform;
	transform.position = {0.0f, 0.0f};
	// gameObject->addComponent<CircleCollider>();
	// CircleCollider* circleCollider = gameObject->getComponents<CircleCollider>()[0];
	// circleCollider->setRadius(10);
	// circleCollider->setTransform(transform);

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
	// ASSERT_NO_THROW(proxy->getCircleColliders());
}

TEST_F(PhysicsTest, AddObjects)
{
	// create game object and check if rigidbody + boxcollider + circlecollider is added
	GameObject* gameObject = new GameObject();
	gameObject->addComponent<RigidBody>();
	gameObject->addComponent<BoxCollider>();
	// gameObject->addComponent<CircleCollider>();
	// gameObject->getComponents<CircleCollider>().at(0)->setRadius(20);
	ASSERT_EQ(gameObject->hasComponent<RigidBody>(), true);
	ASSERT_EQ(gameObject->hasComponent<BoxCollider>(), true);
	// ASSERT_EQ(gameObject->hasComponent<CircleCollider>(), true);

	// Check size of game objects in engine before add
	ASSERT_EQ(mPhysicsEngine.getObjects().size(), 0);

	mPhysicsEngine.addObject(*gameObject);

	// Check size of game objects in engine after add
	ASSERT_EQ(mPhysicsEngine.getObjects().size(), 1);

	GameObject tempObject = mPhysicsEngine.getObjects().at(0);

	// Retrieve earlier added object and check if value remains correct
	// ASSERT_EQ(tempObject.getComponents<CircleCollider>().at(0)->getRadius(), 20);
}

TEST_F(PhysicsTest, updateloop)
{
	// Create object with rigidbody + circlecollider + boxcollider for tests
	GameObject* gameObject = new GameObject();

	gameObject->addComponent<RigidBody>();
	gameObject->addComponent<BoxCollider>();
	gameObject->addComponent<CircleCollider>();

	// set values for rigidbody
	RigidBody* rigidBody = gameObject->getComponents<RigidBody>().at(0);
	rigidBody->setIsMoveableByForce(true);
	rigidBody->setDensity(1.0f);
	rigidBody->setFriction(0.6f);
	rigidBody->setRestitution(0.0f);

	BoxCollider* boxCollider = gameObject->getComponents<BoxCollider>()[0];
	boxCollider->setWidth(10);
	boxCollider->setHeight(10);

	CircleCollider* circleCollider = gameObject->getComponents<CircleCollider>()[0];
	circleCollider->setRadius(10);

	ASSERT_EQ(rigidBody->getBodyId().bodyID, -1);

	mPhysicsEngine.addObject(*gameObject);

	mPhysicsEngine.update();

	std::reference_wrapper<GameObject> tempObject = mPhysicsEngine.getObjects().at(0);
	RigidBody* tempRigidBody = tempObject.get().getComponents<RigidBody>().at(0);

	std::cout << tempRigidBody->getBodyId().bodyID << std::endl;
	ASSERT_EQ(rigidBody->getBodyId().bodyID, 1);
}

TEST_F(PhysicsTest, TranslationStep) {}

TEST_F(PhysicsTest, UpdateFlag)
{
	// Create object with rigidbody + circlecollider + boxcollider for tests
	GameObject* gameObject = new GameObject();

	gameObject->addComponent<RigidBody>();
	gameObject->addComponent<BoxCollider>();
	// gameObject->addComponent<CircleCollider>();

	// set values for rigidbody
	RigidBody* rigidBody = gameObject->getComponents<RigidBody>().at(0);
	rigidBody->setIsMoveableByForce(true);
	rigidBody->setDensity(1.0f);
	rigidBody->setFriction(0.6f);
	rigidBody->setRestitution(0.0f);

	BoxCollider* boxCollider = gameObject->getComponents<BoxCollider>()[0];
	boxCollider->setWidth(10);
	boxCollider->setHeight(10);

	// CircleCollider* circleCollider = gameObject->getComponents<CircleCollider>()[0];
	// circleCollider->setRadius(10);

	mPhysicsEngine.addObject(*gameObject);

	mPhysicsEngine.update();

	b2BodyId bodyID = mPhysicsEngine.getWorld().convertToB2BodyID(rigidBody->getBodyId());
	b2ShapeId shapeArray[gameObject->getComponents<BoxCollider>().size()];

	b2Body_GetShapes(bodyID, shapeArray, gameObject->getComponents<BoxCollider>().size());

	// Check if filter for default constructor is correct
	b2Filter tempFilter = b2Shape_GetFilter(shapeArray[0]);
	ASSERT_EQ(tempFilter.categoryBits, 2);
	ASSERT_EQ(tempFilter.maskBits, 2);

	// Change filter values
	boxCollider = gameObject->getComponents<BoxCollider>()[0];
	boxCollider->setCollideCategory(4);
	boxCollider->setCollideWithCategory({5, 6, 7});
	mPhysicsEngine.update();
	bodyID = mPhysicsEngine.getWorld().convertToB2BodyID(rigidBody->getBodyId());
	b2ShapeId shapeArray2[gameObject->getComponents<BoxCollider>().size()];
	b2Body_GetShapes(bodyID, shapeArray2, gameObject->getComponents<BoxCollider>().size());

	// Check if filter is updated
	tempFilter = b2Shape_GetFilter(shapeArray2[0]);
	ASSERT_EQ(tempFilter.categoryBits, 16);
	ASSERT_EQ(tempFilter.maskBits, 224);
}

TEST_F(PhysicsTest, PositionTranslate)
{
	// Create object with rigidbody + circlecollider + boxcollider for tests
	GameObject* gameObject = new GameObject();

	gameObject->addComponent<RigidBody>();
	gameObject->addComponent<BoxCollider>();
	// gameObject->addComponent<CircleCollider>();

	// set values for rigidbody
	RigidBody* rigidBody = gameObject->getComponents<RigidBody>().at(0);
	rigidBody->setIsMoveableByForce(true);
	rigidBody->setDensity(1.0f);
	rigidBody->setFriction(0.6f);
	rigidBody->setRestitution(0.0f);

	BoxCollider* boxCollider = gameObject->getComponents<BoxCollider>()[0];
	boxCollider->setWidth(10);
	boxCollider->setHeight(10);

	// CircleCollider* circleCollider = gameObject->getComponents<CircleCollider>()[0];
	// circleCollider->setRadius(10);

	mPhysicsEngine.addObject(*gameObject);

	mPhysicsEngine.update();

	std::reference_wrapper<GameObject> tempObject = mPhysicsEngine.getObjects().at(0);
	RigidBody* tempRigidBody = tempObject.get().getComponents<RigidBody>().at(0);

	// Check if position is correctly translated
	ASSERT_EQ(tempRigidBody->getBodyId().bodyID, 1);
	ASSERT_EQ(tempObject.get().getTransform().position,
			  mPhysicsEngine.getWorld().getPosition(tempRigidBody->getBodyId()));
}

TEST_F(PhysicsTest, testcollide)
{
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
	boxCollider->setWidth(8);	// 16
	boxCollider->setHeight(25); // 25
	boxCollider->setTransform(transform);
	gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, 1));

	Transform transform2;
	transform2.position = {0.0f, 0.0f};
	Transform objectTransform2;
	objectTransform2.position.x = 100;
	objectTransform2.position.y = 100;

	gameObject->setTransform(objectTransform);

	GameObject* gameObject2 = new GameObject();

	// gameObject2->addComponent<CircleCollider>();

	// CircleCollider* circleCollider = gameObject2->getComponents<CircleCollider>()[0];
	// circleCollider->setRadius(10);
	// circleCollider->setTransform(transform2);

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

	mPhysicsEngine.addObject(*gameObject);
	mPhysicsEngine.addObject(*gameObject2);

	mPhysicsEngine.setSubStep(4);
	mPhysicsEngine.setStep(1.0f / 60.0f);

	ASSERT_NO_THROW(mPhysicsEngine.update());

	std::vector<std::pair<int, int>> test = mPhysicsEngine.getWorld().getContactEvents();

	for (int i = 0; i < test.size(); i++)
	{
		std::cout << test[i].first << " " << test[i].second << std::endl;
	}

	std::vector<std::pair<int, int>> bodyIDs;
	std::pair<int, int> collide;
	collide.first = 1;
	collide.second = 2;

	bodyIDs.push_back(collide);

	mPhysicsEngine.executeCollisionScripts(bodyIDs);
}

TEST_F(PhysicsTest, BoxCollider)
{
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

TEST_F(PhysicsTest, CircleCollider)
{
	CircleCollider* circleCollider = new CircleCollider(10);
	CircleCollider* circleCollider2 = new CircleCollider();

	circleCollider->setRadius(20);
	ASSERT_EQ(circleCollider->getRadius(), 20);

	ASSERT_NO_THROW(circleCollider->clone());
}

TEST_F(PhysicsTest, remove)
{
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
	boxCollider->setWidth(8);	// 16
	boxCollider->setHeight(25); // 25
	boxCollider->setTransform(transform);
	gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));
	gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));
	gameObject->getComponents<RigidBody>().at(0)->addForce(Vector2(0, -200));

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

	mPhysicsEngine.addObject(*gameObject);
	mPhysicsEngine.addObject(*gameObject2);

	ASSERT_NO_THROW(mPhysicsEngine.update());

	std::vector<std::pair<int, int>> bodyIDs;
	std::pair<int, int> collide;
	collide.first = 1;
	collide.second = 2;

	bodyIDs.push_back(collide);

	mPhysicsEngine.executeCollisionScripts(bodyIDs);

	mPhysicsEngine.removeObject(*gameObject);
	mPhysicsEngine.removeObject(*gameObject2);

	mPhysicsEngine.executeCollisionScripts(bodyIDs);
}
