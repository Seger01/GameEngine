#include "MapToGraph.h"
#include "TileMapParser.h"
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// Test fixture for MapToGraph
class MapToGraphTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Create a sample JSON file for testing including graph layer
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
                    "name": "Graph"
                },
                {
                    "type": "objectgroup",
                    "objects": [
                        {"x": 10, "y": 20, "width" : 50, "height" : 50, "properties": [{"name": "isPlayerSpawn", "type": "bool", "value": true}]}
                    ]
                }
            ]
        })";
		file.close();
	}

	void TearDown() override
	{
		// Remove the sample JSON file after testing
		std::remove("test_map.json");
	}
};

TEST_F(MapToGraphTest, ConvertToGraph)
{
	TileMapParser parser("test_map.json");
	parser.parse();

	const TileMapData& data = parser.getTileMapData();

	// Print layer information for debugging
	for (size_t i = 0; i < data.mLayers.size(); ++i)
	{
		std::cout << "Layer " << i << " Name: " << data.mLayerNames[i] << std::endl;
	}

	MapToGraph mapToGraph(data);
	mapToGraph.convertToGraph();

	const auto& adjacencyList = mapToGraph.getAdjacencyList();

	ASSERT_EQ(adjacencyList.size(), 4);

	// Check connections for each node
	EXPECT_EQ(adjacencyList.at(0).size(), 2);
	EXPECT_EQ(adjacencyList.at(1).size(), 2);
	EXPECT_EQ(adjacencyList.at(2).size(), 2);
	EXPECT_EQ(adjacencyList.at(3).size(), 2);
}

TEST_F(MapToGraphTest, NoGraphLayer) {
    // Create a sample JSON file without a graph layer
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
                "name": "Floor"
            },
            {  
                "type": "objectgroup",
                "objects": [
                    {"x": 10, "y": 20, "width" : 50, "height" : 50, "properties": [{"name": "isPlayerSpawn", "type": "bool", "value": true}]}
                ]
                            }
        ]
    })";
    file.close();

    TileMapParser parser("test_no_graph_layer.json");
    parser.parse();
    
    const TileMapData& data = parser.getTileMapData();
    MapToGraph mapToGraph(data);
    
    // Expect an exception to be thrown when converting to graph
    EXPECT_THROW(mapToGraph.convertToGraph(), std::runtime_error);

    // Remove the sample JSON file after testing
    std::remove("test_no_graph_layer.json");
}
