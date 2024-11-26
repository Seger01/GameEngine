#pragma once

#include "TileMapParser.h"
#include <vector>
#include <unordered_map>
#include <utility>

class MapToGraph {
public:
    // Constructor that takes TileMapData as input
    MapToGraph(const TileMapData& tileMapData);

    // Method to convert the tilemap to a graph
    void convertToGraph();

    // Method to get the adjacency list of the graph
    const std::unordered_map<int, std::vector<int>>& getAdjacencyList() const;

    // Method to print the graph (for debugging purposes)
    void printGraph() const;

private:
    void addEdge(int from, int to);
    bool hasColliders(int tileID) const;

private:
    const TileMapData& mTileMapData;
    std::unordered_map<int, std::vector<int>> mAdjacencyList;

};