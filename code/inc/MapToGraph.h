#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "TileMapParser.h"

class MapToGraph {
public:
    // Constructor
    MapToGraph(const TileMapData& tileMapData);

    // Main graph conversion method
    void convertToGraph();

    // Accessor methods
    const std::unordered_map<int, std::vector<int>>& getAdjacencyList() const;
    void printGraph() const;

private:
    const TileMapData& mTileMapData;
    std::unordered_map<int, std::vector<int>> mAdjacencyList;
    std::unordered_set<int> mWalkableTiles;

    // Directions: Up, Down, Left, Right (optional: add diagonals)
    const std::vector<std::pair<int, int>> mDirections = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

private:
    // Private helper methods
    void preprocessWalkableTiles();
    const std::vector<std::vector<int>>* findFloorLayer() const;
    int calculateNodeIndex(size_t row, size_t col, size_t width) const;
    void connectAdjacentNodes(int currentNode, size_t row, size_t col, const std::vector<std::vector<int>>& layer);
    bool isWalkableTile(int tileID) const;
    void addEdge(int from, int to);
};