#include "Pathfinding.h"
#include <cmath>
#include <unordered_set>
#include <algorithm>

Pathfinding::Pathfinding(const std::unordered_map<int, std::vector<int>>& adjacencyList, int mapWidth, int mapHeight)
    : mAdjacencyList(adjacencyList), mMapWidth(mapWidth), mMapHeight(mapHeight) {}

std::vector<int> Pathfinding::findPath(int start, int goal) {
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> openSet;
    openSet.emplace(0.0, start);

    std::unordered_map<int, int> cameFrom;
    std::unordered_map<int, double> gScore;
    gScore[start] = 0.0;

    std::unordered_map<int, double> fScore;
    fScore[start] = heuristic(start, goal);

    std::unordered_set<int> openSetHash;
    openSetHash.insert(start);

    while (!openSet.empty()) {
        int current = openSet.top().second;
        openSet.pop();
        openSetHash.erase(current);

        if (current == goal) {
            return reconstructPath(cameFrom, current);
        }

        for (int neighbor : mAdjacencyList.at(current)) {
            double tentative_gScore = gScore[current] + 1.0; // Assuming all edges have the same cost

            if (tentative_gScore < gScore[neighbor] || gScore.find(neighbor) == gScore.end()) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, goal);

                if (openSetHash.find(neighbor) == openSetHash.end()) {
                    openSet.emplace(fScore[neighbor], neighbor);
                    openSetHash.insert(neighbor);
                }
            }
        }
    }

    return {}; // Return an empty path if no path is found
}

double Pathfinding::heuristic(int node, int goal) const {
    int nodeX = node % mMapWidth;
    int nodeY = node / mMapWidth;
    int goalX = goal % mMapWidth;
    int goalY = goal / mMapWidth;
    return std::sqrt(std::pow(nodeX - goalX, 2) + std::pow(nodeY - goalY, 2)); // Euclidean distance
}

std::vector<int> Pathfinding::reconstructPath(const std::unordered_map<int, int>& cameFrom, int current) const {
    std::vector<int> path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    path.push_back(current);
    std::reverse(path.begin(), path.end());
    return path;
}