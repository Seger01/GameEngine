#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

class Pathfinding {
public:
    Pathfinding(const std::unordered_map<int, std::vector<int>>& adjacencyList, int mapWidth, int mapHeight);

    // Method to find a path using the A* algorithm
    std::vector<int> findPath(int start, int goal);

private:
    // Heuristic function for A* (Euclidean distance)
    double heuristic(int node, int goal) const;
    
    std::vector<int> reconstructPath(const std::unordered_map<int, int>& cameFrom, int current) const;

private:
    const std::unordered_map<int, std::vector<int>>& mAdjacencyList;
    int mMapWidth;
    int mMapHeight;
};