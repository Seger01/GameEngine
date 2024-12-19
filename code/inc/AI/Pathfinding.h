/**
 * @file Pathfinding.h
 * @brief This file contains the declaration of the Pathfinding class
 * @details The Pathfinding class is responsible for finding the shortest path between two nodes in a graph using the A* algorithm using the Manhatten method to calculate the distance.
 * The graph is represented as an adjacency list.
 */

#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

/**
 * @brief Class for finding the shortest path between two nodes in a graph using the A* algorithm.
 * @details 
 * The graph is represented as an adjacency list.
 * Use findPath() to find the shortest path between two nodes.
 * @param aAdjacencyList: A const reference to an adjacency list representing the graph.
 * @param aMapWidth: The width of the map.
 * @param aMapHeight: The height of the map.
 */
class Pathfinding
{
public:
	Pathfinding(const std::unordered_map<int, std::vector<int>>& aAdjacencyList, int aMapWidth, int aMapHeight);
	std::vector<int> findPath(int aStart, int aGoal) const;
	std::unordered_map<int, std::vector<int>> getAdjacencyList() const;
	int getMapWidth() const;
	int getMapHeight() const;

private:
	double distance(int aNodeA, int aNodeB) const;
	std::vector<int> reconstructPath(const std::unordered_map<int, int>& aCameFrom, int aCurrent) const;

private:
	/// @brief The adjacency list representing the graph gotten from the MapToGraph class.
	std::unordered_map<int, std::vector<int>> mAdjacencyList;

	/// @brief The width of the map.
	int mMapWidth;

	/// @brief The height of the map.
	int mMapHeight;
};