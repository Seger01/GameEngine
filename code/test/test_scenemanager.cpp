#define protected public
#define private public
#include "Scene.h"
#include "SceneManager.h"
#undef protected
#undef private
#include <gtest/gtest.h>
#include <stdexcept>

class SceneManagerTest : public ::testing::Test
{
protected:
	SceneManager sceneManager;

	void SetUp() override
	{
		// Optionally initialize any required setup before tests
	}

	void TearDown() override
	{
		// Optionally clean up after tests
	}
};

// Test creating a scene
TEST_F(SceneManagerTest, CreateScene)
{
	Scene& scene = sceneManager.createScene("TestScene", 1);
	EXPECT_EQ(scene.getName(), "TestScene");
	EXPECT_EQ(scene.getID(), 1);
	EXPECT_TRUE(sceneManager.sceneNameExists("TestScene"));
	EXPECT_TRUE(sceneManager.sceneIDExists(1));
}

// Test creating a scene with an automatically assigned ID
TEST_F(SceneManagerTest, CreateSceneWithAutoID)
{
	Scene& scene = sceneManager.createScene("AutoIDScene", -1);
	EXPECT_EQ(scene.getName(), "AutoIDScene");
	EXPECT_TRUE(scene.getID() != -1);
	EXPECT_TRUE(sceneManager.sceneNameExists("AutoIDScene"));
	EXPECT_TRUE(sceneManager.sceneIDExists(scene.getID()));
}

// Test removing a scene
TEST_F(SceneManagerTest, RemoveScene)
{
	sceneManager.createScene("ToRemoveScene", 2);
	EXPECT_TRUE(sceneManager.sceneNameExists("ToRemoveScene"));

	sceneManager.removeScene("ToRemoveScene");
	EXPECT_FALSE(sceneManager.sceneNameExists("ToRemoveScene"));
}

// Test requesting a scene change by name
TEST_F(SceneManagerTest, RequestSceneChangeByName)
{
	sceneManager.createScene("Scene1", 3);
	sceneManager.createScene("Scene2", 4);

	sceneManager.requestSceneChange("Scene2");
	EXPECT_TRUE(sceneManager.sceneChanged());
	EXPECT_EQ(sceneManager.getCurrentScene().getName(), "Scene2");
}

// Test requesting a scene change by ID
TEST_F(SceneManagerTest, RequestSceneChangeByID)
{
	sceneManager.createScene("Scene1", 5);
	sceneManager.createScene("Scene2", 6);

	sceneManager.requestSceneChange(5);
	sceneManager.update();
	EXPECT_EQ(sceneManager.getCurrentScene().getID(), 5);
}

// Test loading a non-existing scene
TEST_F(SceneManagerTest, LoadNonExistingSceneThrowsException)
{
	EXPECT_THROW(sceneManager.loadScene("NonExistingScene"), std::runtime_error);
}

// Test updating the SceneManager
TEST_F(SceneManagerTest, UpdateSceneManager)
{
	// Scene& scene = sceneManager.createScene("UpdateScene", 7);
	// bool updateCalled = false;
	//
	// // Mock the scene's update function
	// // scene.update = [&]() { updateCalled = true; };
	//
	// sceneManager.requestSceneChange("UpdateScene");
	// sceneManager.update();
	//
	// EXPECT_TRUE(updateCalled);
}

// Test that a unique ID is generated for new scenes
TEST_F(SceneManagerTest, GenerateUniqueSceneID)
{
	int id1 = sceneManager.getNewSceneID();
	sceneManager.createScene("SceneWithUniqueID1", id1);

	int id2 = sceneManager.getNewSceneID();
	EXPECT_NE(id1, id2);
}

// Test handling empty SceneManager
TEST_F(SceneManagerTest, UpdateEmptySceneManagerDoesNothing) { EXPECT_NO_THROW(sceneManager.update()); }
