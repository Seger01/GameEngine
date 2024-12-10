#include "MapToGraph.h"
#include "TileMapParser.h"
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

class MapToGraphTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::ofstream file("test_map.json");
        if (!file)
        {
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
                            "id": 0
                        },
                        {
                            "id": 1,
                            "properties": [{"name": "collider", "type": "bool", "value": true}]
                        },
                        {
                            "id": 2
                        },
                        {
                            "id": 3,
                            "properties": [{"name": "collider", "type": "bool", "value": true}]
                        }
                    ]
                }
            ],
            "layers": [
                {
                    "type": "tilelayer",
                    "width": 4,
                    "height": 4,
                    "data": [
                        0, 2, 0, 0,
                        2, 0, 2, 0,
                        0, 2, 0, 2,
                        0, 0, 2, 0
                    ],
                    "name": "Graph"
                }
            ]
        })";
        file.close();
    }

    void TearDown() override
    {
        std::remove("test_map.json");
    }
};

TEST_F(MapToGraphTest, ConvertToGraph)
{
    TileMapParser parser("test_map.json");
    parser.parse();

    const TileMapData& data = parser.getTileMapData();

    MapToGraph mapToGraph(data);
    mapToGraph.convertToGraph();

    const auto& adjacencyList = mapToGraph.getAdjacencyList();

    ASSERT_EQ(adjacencyList.size(), 6);

    // Check connections for each non-zero node
    EXPECT_EQ(adjacencyList.at(1).size(), 2); // Node 1 (Tile ID 2)
    EXPECT_EQ(adjacencyList.at(4).size(), 2); // Node 4 (Tile ID 2)
    EXPECT_EQ(adjacencyList.at(6).size(), 3); // Node 6 (Tile ID 2)
    EXPECT_EQ(adjacencyList.at(9).size(), 3); // Node 9 (Tile ID 2)
    EXPECT_EQ(adjacencyList.at(11).size(), 2); // Node 11 (Tile ID 2)
    EXPECT_EQ(adjacencyList.at(14).size(), 2); // Node 14 (Tile ID 2)
}

TEST_F(MapToGraphTest, NoGraphLayer) {
    std::ofstream file("test_no_graph_layer.json");
    if (!file) {
        std::cerr << "Error opening file for writing" << std::endl;
        FAIL() << "Failed to open test_no_graph_layer.json";
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
                        "id": 0
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
                "name": "Floor"
            }
        ]
    })";
    file.close();

    TileMapParser parser("test_no_graph_layer.json");
    parser.parse();
    
    const TileMapData& data = parser.getTileMapData();
    MapToGraph mapToGraph(data);
    
    EXPECT_THROW(mapToGraph.convertToGraph(), std::runtime_error);

    std::remove("test_no_graph_layer.json");
}