#include "Pathfinding.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_set>

Pathfinding::Pathfinding(const std::unordered_map<int, std::vector<int>>& aAdjacencyList, int aMapWidth, int aMapHeight)
	: mAdjacencyList(aAdjacencyList), mMapWidth(aMapWidth), mMapHeight(aMapHeight)
{
}

// Find the path from the start node to the goal node using A*
std::vector<int> Pathfinding::findPath(int aStart, int aGoal) const
{
	std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> openSet;
	openSet.emplace(0.0, aStart);
	std::unordered_map<int, int> cameFrom;
	std::unordered_map<int, double> g;
	g[aStart] = 0.0;
	std::unordered_map<int, double> f;
	f[aStart] = distance(aStart, aGoal);
	std::unordered_set<int> openSetHash;
	openSetHash.insert(aStart);

	while (!openSet.empty())
	{
		int current = openSet.top().second;
		openSet.pop();
		openSetHash.erase(current);

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

				if (openSetHash.find(neighbor) == openSetHash.end())
				{
					openSet.emplace(f[neighbor], neighbor);
					openSetHash.insert(neighbor);
				}
			}
		}
	}

	return {};
}

// Find the Manhattan distance between two nodes
double Pathfinding::distance(int aNodeA, int aNodeB) const
{
	int xA = aNodeA % mMapWidth;
	int yA = aNodeA / mMapWidth;
	int xB = aNodeB % mMapWidth;
	int yB = aNodeB / mMapWidth;
	return std::abs(xA - xB) + std::abs(yA - yB);
}

// Reconstruct the path from the cameFrom map
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