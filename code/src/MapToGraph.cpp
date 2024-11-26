#include "MapToGraph.h"
#include <iostream>

MapToGraph::MapToGraph(const TileMapData& tileMapData)
    : mTileMapData(tileMapData) {}

void MapToGraph::convertToGraph() {
    // Iterate over the layers and convert them to a graph
    for (const auto& layer : mTileMapData.mLayers) {
        for (size_t row = 0; row < layer.size(); ++row) {
            for (size_t col = 0; col < layer[row].size(); ++col) {
                int tileID = layer[row][col];
                if (tileID != 0 && !hasColliders(tileID)) { // Assuming 0 is an empty tile
                    int currentNode = row * layer[row].size() + col;

                    // Add edges to adjacent tiles
                    if (row > 0 && layer[row - 1][col] != 0) {
                        int aboveNode = (row - 1) * layer[row].size() + col;
                        addEdge(currentNode, aboveNode);
                    }
                    if (row < layer.size() - 1 && layer[row + 1][col] != 0) {
                        int belowNode = (row + 1) * layer[row].size() + col;
                        addEdge(currentNode, belowNode);
                    }
                    if (col > 0 && layer[row][col - 1] != 0) {
                        int leftNode = row * layer[row].size() + (col - 1);
                        addEdge(currentNode, leftNode);
                    }
                    if (col < layer[row].size() - 1 && layer[row][col + 1] != 0) {
                        int rightNode = row * layer[row].size() + (col + 1);
                        addEdge(currentNode, rightNode);
                    }
                }
            }
        }
    }
}

const std::unordered_map<int, std::vector<int>>& MapToGraph::getAdjacencyList() const {
    return mAdjacencyList;
}

void MapToGraph::printGraph() const {
    for (const auto& pair : mAdjacencyList) {
        std::cout << "Node " << pair.first << ": ";
        for (int neighbor : pair.second) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

void MapToGraph::addEdge(int from, int to) {
    mAdjacencyList[from].push_back(to);
    mAdjacencyList[to].push_back(from); // Assuming an undirected graph
}

bool MapToGraph::hasColliders(int tileID) const {
    auto it = mTileMapData.mTileInfoMap.find(tileID);
    if (it != mTileMapData.mTileInfoMap.end()) {
        const TileInfo& tileInfo = it->second;
        return !tileInfo.mColliders.empty();
    }
    return false;
}