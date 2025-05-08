/**
 * @file Pathfinding.cpp
 * @brief This file contains the implementation of the Pathfinding class
 */

#include "AI/Pathfinding.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_set>

/**
 * @brief Construct a new Pathfinding object
 *
 * @param aAdjacencyList
 * @param aMapWidth
 * @param aMapHeight
 */
Pathfinding::Pathfinding(const std::unordered_map<int, std::vector<int>>& aAdjacencyList, int aMapWidth, int aMapHeight)
	: mAdjacencyList(aAdjacencyList), mMapWidth(aMapWidth), mMapHeight(aMapHeight)
{
}

/**
 * @brief Finds the shortest path between two nodes using the A* algorithm
 *
 * @param aStart
 * @param aGoal
 * @return std::vector<int>
 */
std::vector<int> Pathfinding::findPath(int aStart, int aGoal) const
{
	std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> openSet;
	openSet.emplace(0.0, aStart);
	std::unordered_map<int, int> cameFrom;
	std::unordered_map<int, double> g;
	g[aStart] = 0.0;
	std::unordered_map<int, double> f;
	f[aStart] = distance(aStart, aGoal);

	while (!openSet.empty())
	{
		int current = openSet.top().second;
		openSet.pop();

		if (current == aGoal)
		{
			return reconstructPath(cameFrom, current);
		}

		for (int neighbor : mAdjacencyList.at(current))
		{
			double tentative_g = g[current] + distance(current, neighbor);

			if (g.find(neighbor) == g.end() || tentative_g < g[neighbor])
			{
				cameFrom[neighbor] = current;
				g[neighbor] = tentative_g;
				f[neighbor] = g[neighbor] + distance(neighbor, aGoal);
				openSet.emplace(f[neighbor], neighbor);
			}
		}
	}

	return {};
}

/**
 * @brief Calculates the Manhattan distance between two nodes
 *
 * @param aNodeA
 * @param aNodeB
 * @return double
 */
double Pathfinding::distance(int aNodeA, int aNodeB) const
{
	int xA = aNodeA % mMapWidth;
	int yA = aNodeA / mMapWidth;
	int xB = aNodeB % mMapWidth;
	int yB = aNodeB / mMapWidth;
	return std::abs(xA - xB) + std::abs(yA - yB);
}

/**
 * @brief Reconstructs the path from the cameFrom map
 *
 * @param aCameFrom
 * @param aCurrent
 * @return std::vector<int>
 */
std::vector<int> Pathfinding::reconstructPath(const std::unordered_map<int, int>& aCameFrom, int aCurrent) const
{
	std::vector<int> path;
	while (aCameFrom.find(aCurrent) != aCameFrom.end())
	{
		path.push_back(aCurrent);
		aCurrent = aCameFrom.at(aCurrent);
	}
	path.push_back(aCurrent);
	std::reverse(path.begin(), path.end());
	return path;
}

/**
 * @brief Get the adjacency list
 *
 * @return std::unordered_map<int, std::vector<int>>
 */
std::unordered_map<int, std::vector<int>> Pathfinding::getAdjacencyList() const { return mAdjacencyList; }

/**
 * @brief Get the map width
 *
 * @return int
 */
int Pathfinding::getMapWidth() const { return mMapWidth; }

/**
 * @brief Get the map height
 *
 * @return int
 */
int Pathfinding::getMapHeight() const { return mMapHeight; }
