#include "TileMapParser.h"
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>


class TileMapParserTest : public ::testing::Test {
protected:
    void SetUp() override {
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
                        {"x": 10, "y": 20, "width": 50, "height": 50, "type": "spawn", "name": "PlayerSpawn", "properties": [{"name": "isPlayerSpawn", "type": "bool", "value": true}]},
                        {"x": 30, "y": 40, "width": 60, "height": 60, "type": "trigger", "name": "RoomEntry", "properties": [{"name": "roomID", "type": "string", "value": "room1"}]},
                        {"x": 50, "y": 60, "width": 70, "height": 70, "type": "floatTest", "name": "FloatTest", "properties": [{"name": "floatValue", "type": "float", "value": 1.23}]},
                        {"x": 70, "y": 80, "width": 80, "height": 80, "type": "intTest", "name": "IntTest", "properties": [{"name": "intValue", "type": "int", "value": 123}]}
                    ]
                }
            ]
        })";
        file.close();
    }

    void TearDown() override {
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

    const auto& mapObjects = parser.getTileMapData().mMapObjects;
    ASSERT_EQ(mapObjects.size(), 4);

    auto mapObject = mapObjects[0];
    EXPECT_EQ(mapObject.x, 10);
    EXPECT_EQ(mapObject.y, 20);
    EXPECT_EQ(mapObject.width, 50);
    EXPECT_EQ(mapObject.height, 50);
    EXPECT_EQ(mapObject.type, "spawn");
    EXPECT_EQ(mapObject.name, "PlayerSpawn");
    ASSERT_EQ(mapObject.properties.size(), 1);
    EXPECT_EQ(mapObject.properties.at("isPlayerSpawn"), "true");

    mapObject = mapObjects[1];
    EXPECT_EQ(mapObject.x, 30);
    EXPECT_EQ(mapObject.y, 40);
    EXPECT_EQ(mapObject.width, 60);
    EXPECT_EQ(mapObject.height, 60);
    EXPECT_EQ(mapObject.type, "trigger");
    EXPECT_EQ(mapObject.name, "RoomEntry");
    ASSERT_EQ(mapObject.properties.size(), 1);
    EXPECT_EQ(mapObject.properties.at("roomID"), "room1");

    mapObject = mapObjects[2];
    EXPECT_EQ(mapObject.x, 50);
    EXPECT_EQ(mapObject.y, 60);
    EXPECT_EQ(mapObject.width, 70);
    EXPECT_EQ(mapObject.height, 70);
    EXPECT_EQ(mapObject.type, "floatTest");
    EXPECT_EQ(mapObject.name, "FloatTest");
    ASSERT_EQ(mapObject.properties.size(), 1);
    EXPECT_EQ(mapObject.properties.at("floatValue"), "1.230000");

    mapObject = mapObjects[3];
    EXPECT_EQ(mapObject.x, 70);
    EXPECT_EQ(mapObject.y, 80);
    EXPECT_EQ(mapObject.width, 80);
    EXPECT_EQ(mapObject.height, 80);
    EXPECT_EQ(mapObject.type, "intTest");
    EXPECT_EQ(mapObject.name, "IntTest");
    ASSERT_EQ(mapObject.properties.size(), 1);
    EXPECT_EQ(mapObject.properties.at("intValue"), "123");
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

TEST_F(TileMapParserTest, InvalidJSON) {
    std::ofstream file("invalid_json.json");
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
                    {"x": 10, "y": 20, "width": 50, "height": 50, "type": "spawn", "name": "PlayerSpawn", "properties": [{"name": "isPlayerSpawn", "type": "bool", "value": true}]},
                    {"x": 30, "y": 40, "width": 60, "height": 60, "type": "trigger", "name": "RoomEntry", "properties": [{"name": "roomID", "type": "string", "value": "room1"}]}
                ]
            }
        ]
    )"; 
    file.close();

    TileMapParser parser("invalid_json.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("invalid_json.json");
}

TEST_F(TileMapParserTest, NullJSON) {
    std::ofstream file("null_json.json");
    file << R"({
        "tilesets": null,
        "layers": null
    })";
    file.close();

    TileMapParser parser("null_json.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("null_json.json");
}

TEST_F(TileMapParserTest, InvalidColliderFormat) {
    std::ofstream file("invalid_collider.json");
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
                                {"x": "invalid", "y": 0, "width": 32, "height": 32}
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
            }
        ]
    })";
    file.close();

    TileMapParser parser("invalid_collider.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("invalid_collider.json");
}

TEST_F(TileMapParserTest, GIDNotFoundInAnyTileset) {
    std::ofstream file("test_map.json");
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
        ],
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

    TileMapParser parser("test_map.json");
    parser.parse();

    // Use a gID that does not exist in the tilesets
    EXPECT_THROW(parser.getTilePosition(999), std::runtime_error);

    std::remove("test_map.json");
}

TEST_F(TileMapParserTest, NullJSONData) {
    std::ofstream file("null_json_data.json");
    file << "null"; // This will result in mJsonData being null
    file.close();

    TileMapParser parser("null_json_data.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("null_json_data.json");
}

TEST_F(TileMapParserTest, MissingTypeKey) {
    std::ofstream file("missing_type_key.json");
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
        ],
        "layers": [
            {
                "width": 2,
                "height": 2,
                "data": [1, 2, 3, 4],
                "name": "Tile Layer 1"
            }
        ]
    })";
    file.close();

    TileMapParser parser("missing_type_key.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("missing_type_key.json");
}

TEST_F(TileMapParserTest, ObjectLayerMissingObjectsKey) {
    std::ofstream file("object_layer_missing_objects_key.json");
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
        ],
        "layers": [
            {
                "type": "objectgroup",
                "name": "Object Layer 1"
            }
        ]
    })";
    file.close();

    TileMapParser parser("object_layer_missing_objects_key.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("object_layer_missing_objects_key.json");
}

TEST_F(TileMapParserTest, ObjectLayerObjectsNotArray) {
    std::ofstream file("object_layer_objects_not_array.json");
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
        ],
        "layers": [
            {
                "type": "objectgroup",
                "objects": "not an array",
                "name": "Object Layer 1"
            }
        ]
    })";
    file.close();

    TileMapParser parser("object_layer_objects_not_array.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("object_layer_objects_not_array.json");
}

TEST_F(TileMapParserTest, ObjectPropertyUnknownType) {
    std::ofstream file("object_property_unknown_type.json");
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
        ],
        "layers": [
            {
                "type": "objectgroup",
                "objects": [
                    {
                        "x": 10,
                        "y": 20,
                        "width": 50,
                        "height": 50,
                        "type": "custom",
                        "name": "CustomObject",
                        "properties": [
                            {
                                "name": "customProperty",
                                "type": "unknownType",
                                "value": {"key": "value"}
                            }
                        ]
                    }
                ],
                "name": "Object Layer 1"
            }
        ]
    })";
    file.close();

    TileMapParser parser("object_property_unknown_type.json");
    EXPECT_THROW(parser.parse(), std::runtime_error);

    std::remove("object_property_unknown_type.json");
}