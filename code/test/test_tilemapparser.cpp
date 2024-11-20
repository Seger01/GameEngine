#include "TileMapParser.h"
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// Test fixture for TileMapParser
class TileMapParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a sample JSON file for testing
        std::ofstream file("test_map.json");
        if (!file) {
            std::cerr << "Error opening file for writing" << std::endl;
            FAIL() << "Failed to open test_map.json";
        }
        file << R"({
            "tilesets": [
                {
                    "firstgid": 1,
                    "tilecount": 4,
                    "columns": 2,
                    "tilewidth": 32,
                    "tileheight": 32,
                    "image": "tileset.png",
                    "tiles": [
                        {
                            "id": 0,
                            "objectgroup": {
                                "objects": [
                                    {"x": 0, "y": 0, "width": 32, "height": 32}
                                ]
                            }
                        }
                    ]
                }
            ],
            "layers": [
                {
                    "type": "tilelayer",
                    "width": 2,
                    "height": 2,
                    "data": [1, 2, 3, 4],
                    "name": "Tile Layer 1"
                },
                {
                    "type": "objectgroup",
                    "objects": [
                        {"x": 10, "y": 20, "properties": [{"name": "isPlayerSpawn", "type": "bool", "value": true}]}
                    ]
                }
            ]
        })";
        file.close();
    }

    void TearDown() override {
        // Remove the sample JSON file after testing
        std::remove("test_map.json");
    }
};

TEST_F(TileMapParserTest, ParseJSONFile) {
    TileMapParser parser("test_map.json");
    parser.parse();

    const TileMapData& data = parser.getTileMapData();
    ASSERT_EQ(data.mLayers.size(), 1);
    ASSERT_EQ(data.mLayers[0].size(), 2);
    ASSERT_EQ(data.mLayers[0][0].size(), 2);
    EXPECT_EQ(data.mLayers[0][0][0], 1);
    EXPECT_EQ(data.mLayers[0][0][1], 2);
    EXPECT_EQ(data.mLayers[0][1][0], 3);
    EXPECT_EQ(data.mLayers[0][1][1], 4);
}

TEST_F(TileMapParserTest, GetTilePosition) {
    TileMapParser parser("test_map.json");
    parser.parse();

    auto position = parser.getTilePosition(1);
    EXPECT_EQ(position.first, 0);
    EXPECT_EQ(position.second, 0);

    position = parser.getTilePosition(2);
    EXPECT_EQ(position.first, 32);
    EXPECT_EQ(position.second, 0);

    position = parser.getTilePosition(3);
    EXPECT_EQ(position.first, 0);
    EXPECT_EQ(position.second, 32);

    position = parser.getTilePosition(4);
    EXPECT_EQ(position.first, 32);
    EXPECT_EQ(position.second, 32);
}

TEST_F(TileMapParserTest, StoreTileInfo) {
    TileMapParser parser("test_map.json");
    parser.parse();

    const auto& tileInfoMap = parser.getTileMapData().mTileInfoMap;
    ASSERT_EQ(tileInfoMap.size(), 4);

    auto tileInfo = tileInfoMap.at(1);
    EXPECT_EQ(tileInfo.mTilesetName, "tileset.png");
    EXPECT_EQ(tileInfo.mCoordinates.first, 0);
    EXPECT_EQ(tileInfo.mCoordinates.second, 0);
    ASSERT_EQ(tileInfo.mColliders.size(), 1);
    EXPECT_EQ(tileInfo.mColliders[0].x, 0);
    EXPECT_EQ(tileInfo.mColliders[0].y, 0);
    EXPECT_EQ(tileInfo.mColliders[0].mWidth, 32);
    EXPECT_EQ(tileInfo.mColliders[0].mHeight, 32);
}

TEST_F(TileMapParserTest, ParseObjectLayer) {
    TileMapParser parser("test_map.json");
    parser.parse();

    const auto& spawnPoints = parser.getTileMapData().mSpawnPoints;
    ASSERT_EQ(spawnPoints.size(), 1);
    EXPECT_EQ(spawnPoints[0].x, 10);
    EXPECT_EQ(spawnPoints[0].y, 20);
    EXPECT_TRUE(spawnPoints[0].isPlayerSpawn);
}

TEST_F(TileMapParserTest, InvalidFilePath) {
    TileMapParser parser("invalid_path.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);
}

TEST_F(TileMapParserTest, EmptyFile) {
    std::ofstream file("empty_map.json");
    file.close();

    TileMapParser parser("empty_map.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("empty_map.json");
}

TEST_F(TileMapParserTest, MissingTilesets) {
    std::ofstream file("missing_tilesets.json");
    file << R"({
        "layers": [
            {
                "type": "tilelayer",
                "width": 2,
                "height": 2,
                "data": [1, 2, 3, 4],
                "name": "Tile Layer 1"
            }
        ]
    })";
    file.close();

    TileMapParser parser("missing_tilesets.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("missing_tilesets.json");
}

TEST_F(TileMapParserTest, MissingLayers) {
    std::ofstream file("missing_layers.json");
    file << R"({
        "tilesets": [
            {
                "firstgid": 1,
                "tilecount": 4,
                "columns": 2,
                "tilewidth": 32,
                "tileheight": 32,
                "image": "tileset.png"
            }
        ]
    })";
    file.close();

    TileMapParser parser("missing_layers.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("missing_layers.json");
}