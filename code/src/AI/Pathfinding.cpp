#include "Pathfinding.h"
#include <cmath>
#include <unordered_set>
#include <algorithm>
#include <iostream>

Pathfinding::Pathfinding(const std::unordered_map<int, std::vector<int>>& adjacencyList, int mapWidth, int mapHeight)
 : mAdjacencyList(adjacencyList), mMapWidth(mapWidth), mMapHeight(mapHeight) {
    // Debug: Print all available nodes in the adjacency list
    std::cout << "Available nodes in adjacency list:" << std::endl;
    for (const auto& [node, neighbors] : mAdjacencyList) {
        std::cout << "Node " << node << " has " << neighbors.size() << " neighbors" << std::endl;
    }
}

std::vector<int> Pathfinding::findPath(int start, int goal) {
    std::cout << "Attempting to find path from " << start << " to " << goal << std::endl;

    // Check if start and goal nodes exist in the adjacency list
    if (mAdjacencyList.find(start) == mAdjacencyList.end()) {
        std::cerr << "Start node " << start << " not found in adjacency list!" << std::endl;
        
        // Debug: Find the closest available node
        int closestNode = findClosestAvailableNode(start);
        if (closestNode != -1) {
            std::cout << "Using closest available node: " << closestNode << std::endl;
            start = closestNode;
        } else {
            return {};
        }
    }

    if (mAdjacencyList.find(goal) == mAdjacencyList.end()) {
        std::cerr << "Goal node " << goal << " not found in adjacency list!" << std::endl;
        
        // Debug: Find the closest available node
        int closestNode = findClosestAvailableNode(goal);
        if (closestNode != -1) {
            std::cout << "Using closest available goal node: " << closestNode << std::endl;
            goal = closestNode;
        } else {
            return {};
        }
    }

    std::cout << "Initializing openSet, gScore, and fScore" << std::endl;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> openSet;
    openSet.emplace(0.0, start);
    std::unordered_map<int, int> cameFrom;
    std::unordered_map<int, double> gScore;
    gScore[start] = 0.0;
    std::unordered_map<int, double> fScore;
    fScore[start] = heuristic(start, goal);
    std::unordered_set<int> openSetHash;
    openSetHash.insert(start);

    std::cout << "Entering while loop" << std::endl;
    while (!openSet.empty()) {
        int current = openSet.top().second;
        openSet.pop();
        openSetHash.erase(current);

        if (current == goal) {
            return reconstructPath(cameFrom, current);
        }

        for (int neighbor : mAdjacencyList.at(current)) {
            double tentative_gScore = gScore[current] + distance(current, neighbor);

            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
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

    return {};
}

int Pathfinding::findClosestAvailableNode(int targetNode) const {
    std::cout << "Finding closest available node to " << targetNode << std::endl;
    int closestNode = -1;
    double minDistance = std::numeric_limits<double>::max();

    for (const auto& [node, neighbors] : mAdjacencyList) {
        double distance = std::abs(node - targetNode);
        if (distance < minDistance) {
            minDistance = distance;
            closestNode = node;
        }
    }

    return closestNode;
}

double Pathfinding::heuristic(int node, int goal) const {
    int nodeX = node % mMapWidth;
    int nodeY = node / mMapWidth;
    int goalX = goal % mMapWidth;
    int goalY = goal / mMapWidth;

    std::cout << "Heuristic: " << std::sqrt(std::pow(nodeX - goalX, 2) + std::pow(nodeY - goalY, 2)) << std::endl;
    return std::sqrt(std::pow(nodeX - goalX, 2) + std::pow(nodeY - goalY, 2)); // Euclidean distance
}

double Pathfinding::distance(int nodeA, int nodeB) const {
    int xA = nodeA % mMapWidth;
    int yA = nodeA / mMapWidth;
    int xB = nodeB % mMapWidth;
    int yB = nodeB / mMapWidth;
    return std::abs(xA - xB) + std::abs(yA - yB);
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