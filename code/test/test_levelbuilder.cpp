#include "EngineBravo.h"
#include "LevelBuilder.h"
#include "Scene.h"
#include "TileMapParser.h"
#include <gtest/gtest.h>

class LevelBuilderTests : public ::testing::Test
{
protected:
	void SetUp() override
	{
		engine = &EngineBravo::getInstance();
		engine->initialize();
	}

	void TearDown() override { engine->getSceneManager().removeScene("TestSceneLevelBuilder"); }

	EngineBravo* engine;
};

TEST_F(LevelBuilderTests, CreateLevel_NullScene_ThrowsException)
{
	LevelBuilder levelBuilder;
	TileMapData tileMapData;
	EXPECT_THROW(levelBuilder.createLevel(nullptr, tileMapData), std::runtime_error);
}


TEST_F(LevelBuilderTests, CreateTileLayers_AddsTiles)
{
	LevelBuilder levelBuilder;
	TileMapData tileMapData;

	tileMapData.mLayers = {{{1, 0}, {0, 1}}};
	tileMapData.mLayerNames = {"Layer1"};
	TileInfo tileInfo;
	tileInfo.mTilesetName = "Dungeontileset/0x72_DungeonTilesetII_v1.7.png";
	tileInfo.mCoordinates = {0, 0};
	tileMapData.mTileInfoMap[0] = tileInfo;

	Scene& scene = engine->getSceneManager().createScene("TestSceneLevelBuilder", 1);
	levelBuilder.createLevel(&scene, tileMapData);

	auto gameObjects = scene.getGameObjects();
	ASSERT_EQ(gameObjects.size(), 2);
	EXPECT_EQ(gameObjects[0].get().getName(), "Tile");
	EXPECT_EQ(gameObjects[1].get().getName(), "Tile");
}

TEST_F(LevelBuilderTests, CreateTileLayers_AddsTilesWithCollidersAndRigidBodies)
{
	LevelBuilder levelBuilder;
	TileMapData tileMapData;

	tileMapData.mLayers = {{{1, 0}, {0, 1}}};
	tileMapData.mLayerNames = {"Layer1"};
	TileInfo tileInfo;
	tileInfo.mTilesetName = "Dungeontileset/0x72_DungeonTilesetII_v1.7.png";
	tileInfo.mCoordinates = {0, 0};

	// Add colliders to the tile info
	ColliderData collider;
	collider.x = 0.0f;
	collider.y = 0.0f;
	collider.mWidth = 16.0f;
	collider.mHeight = 16.0f;
	tileInfo.mColliders.push_back(collider);

	tileMapData.mTileInfoMap[0] = tileInfo;

	Scene& scene = engine->getSceneManager().createScene("TestSceneLevelBuilder", 1);
	levelBuilder.createLevel(&scene, tileMapData);

	auto gameObjects = scene.getGameObjects();
	ASSERT_EQ(gameObjects.size(), 2);
	EXPECT_EQ(gameObjects[0].get().getName(), "Tile");
	EXPECT_EQ(gameObjects[1].get().getName(), "Tile");

	// Verify that the game objects have colliders and rigid bodies

	auto boxColliders = gameObjects[0].get().getComponents<BoxCollider>();
	auto rigidBodies = gameObjects[1].get().getComponents<RigidBody>();
	EXPECT_FALSE(boxColliders.empty());
	EXPECT_FALSE(rigidBodies.empty());
}