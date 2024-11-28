#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

class Pathfinding
{
public:
	Pathfinding(const std::unordered_map<int, std::vector<int>>& aAdjacencyList, int aMapWidth, int aMapHeight);
	std::vector<int> findPath(int aStart, int aGoal);

private:
	std::unordered_map<int, std::vector<int>> mAdjacencyList;
	int mMapWidth;
	int mMapHeight;

private:
	double distance(int aNodeA, int aNodeB) const;
	double heuristic(int aNode, int aGoal) const;
	std::vector<int> reconstructPath(const std::unordered_map<int, int>& aCameFrom, int aCurrent) const;
};