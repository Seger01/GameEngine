#pragma once

#include <unordered_map>
#include <vector>
#include <queue>

class Pathfinding {
private:
    std::unordered_map<int, std::vector<int>> mAdjacencyList;
    int mMapWidth;
    int mMapHeight;

    // Helper method to find the closest available node
    int findClosestAvailableNode(int targetNode) const;
    double distance(int nodeA, int nodeB) const;
    double heuristic(int node, int goal) const;
    std::vector<int> reconstructPath(const std::unordered_map<int, int>& cameFrom, int current) const;

public:
    Pathfinding(const std::unordered_map<int, std::vector<int>>& adjacencyList, int mapWidth, int mapHeight);
    
    std::vector<int> findPath(int start, int goal);
};