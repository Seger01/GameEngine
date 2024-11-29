#include "Pathfinding.h"
#include <gtest/gtest.h>
#include <unordered_map>
#include <vector>

// Test fixture for Pathfinding
class PathfindingTest : public ::testing::Test {
protected:
    void SetUp() override {
        adjacencyList = {
            {0, {1, 3}},       // Node (0,0)
            {1, {0, 2, 4}},    // Node (0,1)
            {2, {1, 5}},       // Node (0,2)
            {3, {0, 4, 6}},    // Node (1,0)
            {4, {1, 3, 5, 7}}, // Node (1,1)
            {5, {2, 4, 8}},    // Node (1,2)
            {6, {3, 7}},       // Node (2,0)
            {7, {4, 6, 8}},    // Node (2,1)
            {8, {5, 7}}        // Node (2,2)
        };
        mapWidth = 3;
        mapHeight = 3;
    }

    std::unordered_map<int, std::vector<int>> adjacencyList;
    int mapWidth;
    int mapHeight;
};

TEST_F(PathfindingTest, FindPath) {
    Pathfinding pathfinding(adjacencyList, mapWidth, mapHeight);
    std::vector<int> path = pathfinding.findPath(0, 8);
    
    std::vector<int> expectedPath = {0, 1, 2, 5, 8};
    ASSERT_EQ(path, expectedPath);
}

TEST_F(PathfindingTest, FindPathNoPath) {
    // Modify adjacency list to create a scenario with no path
    adjacencyList = {
        {0, {1}},
        {1, {0}},
        {2, {3}},
        {3, {2}}
    };
    Pathfinding pathfinding(adjacencyList, mapWidth, mapHeight);
    std::vector<int> path = pathfinding.findPath(0, 3);
    
    std::vector<int> expectedPath = {};
    ASSERT_EQ(path, expectedPath);
}
