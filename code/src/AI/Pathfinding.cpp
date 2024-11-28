#include "Pathfinding.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_set>

Pathfinding::Pathfinding(const std::unordered_map<int, std::vector<int>>& aAdjacencyList, int aMapWidth, int aMapHeight)
	: mAdjacencyList(aAdjacencyList), mMapWidth(aMapWidth), mMapHeight(aMapHeight)
{
}


//Find the path from the start node to the goal node using A*
std::vector<int> Pathfinding::findPath(int aStart, int aGoal)
{
	std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> openSet;
	openSet.emplace(0.0, aStart);
	std::unordered_map<int, int> cameFrom;
	std::unordered_map<int, double> gScore;
	gScore[aStart] = 0.0;
	std::unordered_map<int, double> fScore;
	fScore[aStart] = heuristic(aStart, aGoal);
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
			double tentative_gScore = gScore[current] + distance(current, neighbor);

			if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor])
			{
				cameFrom[neighbor] = current;
				gScore[neighbor] = tentative_gScore;
				fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, aGoal);

				if (openSetHash.find(neighbor) == openSetHash.end())
				{
					openSet.emplace(fScore[neighbor], neighbor);
					openSetHash.insert(neighbor);
				}
			}
		}
	}

	return {};
}


//Find the euclidean distance between two nodes
double Pathfinding::heuristic(int aNode, int aGoal) const
{
	int nodeX = aNode % mMapWidth;
	int nodeY = aNode / mMapWidth;
	int goalX = aGoal % mMapWidth;
	int goalY = aGoal / mMapWidth;

	return std::sqrt(std::pow(nodeX - goalX, 2) + std::pow(nodeY - goalY, 2)); // Euclidean distance
}


double Pathfinding::distance(int aNodeA, int aNodeB) const
{
	int xA = aNodeA % mMapWidth;
	int yA = aNodeA / mMapWidth;
	int xB = aNodeB % mMapWidth;
	int yB = aNodeB / mMapWidth;
	return std::abs(xA - xB) + std::abs(yA - yB);
}


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