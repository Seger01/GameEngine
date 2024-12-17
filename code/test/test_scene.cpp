#include "Camera.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "Scene.h"
#include <gtest/gtest.h>
#include <stdexcept>

// Scene tests
class SceneTest : public ::testing::Test
{
protected:
	void SetUp() override {}

	void TearDown() override { EngineBravo::getInstance().getSceneManager().removeScene("TestScene"); }

	Scene& scene { EngineBravo::getInstance().getSceneManager().createScene("TestScene", 1); };
};

// Test creation of Scene
TEST_F(SceneTest, SceneInitialization)
{
	EXPECT_EQ(scene->getName(), "TestScene");
	EXPECT_EQ(scene->getID(), 1);
}

// Test adding and retrieving GameObjects
TEST_F(SceneTest, AddAndGetGameObjects)
{
	auto* obj1 = new GameObject();
	auto* obj2 = new GameObject();

	obj1->setID(1);
	obj2->setID(2);

	scene->addGameObject(obj1);
	scene->addGameObject(obj2);

	auto gameObjects = scene->getGameObjects();
	EXPECT_EQ(gameObjects.size(), 2);
	EXPECT_EQ(gameObjects[0].get().getID(), 1);
	EXPECT_EQ(gameObjects[1].get().getID(), 2);
}

// Test retrieving GameObjects by tag
TEST_F(SceneTest, GetGameObjectsByTag)
{
	auto* obj1 = new GameObject();
	auto* obj2 = new GameObject();

	obj1->setTag("Player");
	obj2->setTag("Enemy");

	obj1->setID(1);
	obj2->setID(2);

	scene->addGameObject(obj1);
	scene->addGameObject(obj2);

	auto players = scene->getGameObjectsWithTag("Player");
	auto enemies = scene->getGameObjectsWithTag("Enemy");

	EXPECT_EQ(players.size(), 1);
	EXPECT_EQ(players[0].get().getID(), 1);

	EXPECT_EQ(enemies.size(), 1);
	EXPECT_EQ(enemies[0].get().getID(), 2);
}

// Test removing GameObjects
TEST_F(SceneTest, RemoveGameObject)
{
	auto* obj1 = new GameObject();

	scene->addGameObject(obj1);
	EXPECT_EQ(scene->getGameObjects().size(), 1);

	scene->requestGameObjectRemoval(obj1);
	EXPECT_EQ(scene->getGameObjects().size(), 1);
}

// Test requesting GameObject removal
TEST_F(SceneTest, RequestGameObjectRemovalByPointer)
{
	auto* obj1 = new GameObject();

	scene->addGameObject(obj1);
	scene->requestGameObjectRemoval(obj1);

	EXPECT_EQ(scene->getGameObjectsToBeRemoved().size(), 1);
}

// Test requesting GameObject removal by ID
TEST_F(SceneTest, RequestGameObjectRemovalByID)
{
	auto* obj1 = new GameObject();
	obj1->setID(1);

	scene->addGameObject(obj1);
	scene->requestGameObjectRemoval(1);

	EXPECT_EQ(scene->getGameObjectsToBeRemoved().size(), 1);
}

// Test adding and removing cameras
TEST_F(SceneTest, AddAndRemoveCameras)
{
	Camera* camera1 = new Camera();
	camera1->setTag("MainCamera");

	scene->addGameObject(camera1);

	auto cameras = scene->getCameraWithTag("MainCamera");

	EXPECT_EQ(cameras->getID(), camera1->getID());
}

// Test setting and getting active camera
TEST_F(SceneTest, ActiveCamera) {}

// Test updating GameObjects
TEST_F(SceneTest, UpdateRemovesGameObjects)
{
	auto* obj1 = new GameObject();
	scene->addGameObject(obj1);

	scene->requestGameObjectRemoval(obj1);
	scene->update();

	EXPECT_EQ(scene->getGameObjects().size(), 0);
}
