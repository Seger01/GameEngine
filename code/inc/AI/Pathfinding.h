#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

/**
 * @brief 
 * Responsible for finding the shortest path between two nodes in a graph using the A* algorithm.
 * The graph is represented as an adjacency list.
 * use findPath() to find the shortest path between two nodes.
 * @param 
 * aAdjacencyList: A const reference to an adjacency list representing the graph.
 * aMapWidth: The width of the map.
 * aMapHeight: The height of the map.
 */
class Pathfinding
{
public:
	Pathfinding(const std::unordered_map<int, std::vector<int>>& aAdjacencyList, int aMapWidth, int aMapHeight);
	std::vector<int> findPath(int aStart, int aGoal) const;

private:
	std::unordered_map<int, std::vector<int>> mAdjacencyList;
	int mMapWidth;
	int mMapHeight;

private:
	double distance(int aNodeA, int aNodeB) const;
	std::vector<int> reconstructPath(const std::unordered_map<int, int>& aCameFrom, int aCurrent) const;
};