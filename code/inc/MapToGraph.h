#pragma once

#include "TileMapParser.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief 
 * Responsible for converting a tilemap graph layer to a graph.
 * Use convertToGraph() to convert the graph layer to a graph.
 * The graph is represented as an adjacency list.
 * @param 
 * aTileMapData: A const reference to a TileMapData object.
 */
class MapToGraph
{
public:
	MapToGraph(const TileMapData& aTileMapData);
	void convertToGraph();
	const std::unordered_map<int, std::vector<int>>& getAdjacencyList() const;
	void printGraph() const;

private:
	void preprocessWalkableTiles();
	const std::vector<std::vector<int>>* findGraphLayer() const;
	int calculateNodeIndex(size_t aRow, size_t aCol, size_t aWidth) const;
	void connectAdjacentNodes(int aCurrentNode, size_t aRow, size_t aCol, const std::vector<std::vector<int>>& aLayer);
	bool isWalkableTile(int aTileID) const;
	void addEdge(int aFrom, int aTo);

private:
	const TileMapData& mTileMapData;
	std::unordered_map<int, std::vector<int>> mAdjacencyList;
	std::unordered_set<int> mWalkableTiles;
	const std::vector<std::pair<int, int>> mDirections;
};