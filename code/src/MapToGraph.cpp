#include "MapToGraph.h"
#include <iostream>

// Constructor
MapToGraph::MapToGraph(const TileMapData& tileMapData) 
    : mTileMapData(tileMapData) {
    preprocessWalkableTiles();
}

// Preprocess walkable tiles
void MapToGraph::preprocessWalkableTiles() {
    for (const auto& [tileID, tileInfo] : mTileMapData.mTileInfoMap) {
        if (tileInfo.mColliders.empty()) {
            mWalkableTiles.insert(tileID);
        }
    }
}

// Find the first non-empty layer
const std::vector<std::vector<int>>* MapToGraph::findFloorLayer() const {
    for (const auto& layer : mTileMapData.mLayers) {
        if (!layer.empty() && !layer[0].empty()) {
            return &layer;
        }
    }
    return nullptr;
}

// Convert tilemap to graph
void MapToGraph::convertToGraph() {
    const auto* floorLayer = findFloorLayer();
    if (!floorLayer) {
        std::cerr << "Floor layer not found!" << std::endl;
        return;
    }

    for (size_t row = 0; row < floorLayer->size(); ++row) {
        for (size_t col = 0; col < (*floorLayer)[row].size(); ++col) {
            int currentTileID = (*floorLayer)[row][col];
           // if (isWalkableTile(currentTileID)) {
                int currentNode = calculateNodeIndex(row, col, floorLayer->at(row).size());
                connectAdjacentNodes(currentNode, row, col, *floorLayer);

                std::cout << "Processing Tile at (" << row << ", " << col << "), ID: " 
                << currentTileID << ", Node Index: " << currentNode << std::endl;
          //  }
        }
    }
}

// Calculate unique node index
int MapToGraph::calculateNodeIndex(size_t row, size_t col, size_t width) const {
    return row * width + col;
}

// Connect adjacent walkable nodes
void MapToGraph::connectAdjacentNodes(int currentNode, size_t row, size_t col, const std::vector<std::vector<int>>& layer) {
    for (const auto& [dx, dy] : mDirections) {
        size_t newRow = row + dx;
        size_t newCol = col + dy;

        // Bounds and walkability check
        if (newRow < layer.size() && 
            newCol < layer[newRow].size() && 
            isWalkableTile(layer[newRow][newCol])) {
            
            int adjacentNode = calculateNodeIndex(newRow, newCol, layer[newRow].size());
            addEdge(currentNode, adjacentNode);
        }
    }
}

// Check if a tile is walkable
bool MapToGraph::isWalkableTile(int tileID) const {
    return tileID != 0 && mWalkableTiles.count(tileID) > 0;
}

// Add an edge between nodes
void MapToGraph::addEdge(int from, int to) {
    mAdjacencyList[from].push_back(to);
    mAdjacencyList[to].push_back(from);
}

// Get the adjacency list
const std::unordered_map<int, std::vector<int>>& MapToGraph::getAdjacencyList() const {
    return mAdjacencyList;
}

// Print the graph for debugging
void MapToGraph::printGraph() const {
    for (const auto& pair : mAdjacencyList) {
        std::cout << "Node " << pair.first << ": ";
        for (int neighbor : pair.second) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}