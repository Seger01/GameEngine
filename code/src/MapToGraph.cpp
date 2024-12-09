#include "MapToGraph.h"
#include <iostream>

/**
 * @brief Construct a new MapToGraph object
 *
 * @param aTileMapData
 */
MapToGraph::MapToGraph(const TileMapData& aTileMapData)
	: mTileMapData(aTileMapData), mDirections{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}
{
	preprocessWalkableTiles();
}

/**
 * @brief Preprocess walkable tiles
 * @details This function processes the walkable tiles in the tilemap data and stores them in a set.
 */
//
void MapToGraph::preprocessWalkableTiles()
{
	for (const auto& [tileID, tileInfo] : mTileMapData.mTileInfoMap)
	{
		if (tileInfo.mColliders.empty())
		{
			mWalkableTiles.insert(tileID);
		}
	}
}

/**
 * @brief Finds the graph layer in the tilemap data
 *
 * @return const std::vector<std::vector<int>>*
 */
const std::vector<std::vector<int>>* MapToGraph::findGraphLayer() const
{
	for (size_t i = 0; i < mTileMapData.mLayers.size(); ++i)
	{
		if (mTileMapData.mLayerNames[i] == "Graph")
		{
			return &mTileMapData.mLayers[i];
		}
	}
	return nullptr;
}

/**
 * @brief Converts the graph layer of the tilemap to a graph
 *
 */
void MapToGraph::convertToGraph()
{
	const auto* graphLayer = findGraphLayer();
	if (graphLayer == nullptr)
	{
		throw std::runtime_error("Graph layer not found in map! (MapToGraph::convertToGraph)");
	}

	for (size_t row = 0; row < graphLayer->size(); ++row)
	{
		for (size_t col = 0; col < (*graphLayer)[row].size(); ++col)
		{
			if (!isWalkableTile((*graphLayer)[row][col]))
			{
				continue;
			}
			int currentNode = calculateNodeIndex(row, col, (*graphLayer)[row].size());
			connectAdjacentNodes(currentNode, row, col, graphLayer);
		}
	}
}

/**
 * @brief Calculates the unique node index
 *
 * @param aRow
 * @param aCol
 * @param aWidth
 * @return int
 */
int MapToGraph::calculateNodeIndex(size_t aRow, size_t aCol, size_t aWidth) const { return aRow * aWidth + aCol; }

/**
 * @brief Connects adjacent walkable nodes
 *
 * @param aCurrentNode
 * @param aRow
 * @param aCol
 * @param aLayer
 */
void MapToGraph::connectAdjacentNodes(int aCurrentNode, size_t aRow, size_t aCol,
									  const std::vector<std::vector<int>>* aLayer)
{
	std::unordered_set<int> uniqueAdjacentNodes;

	for (const auto& [dx, dy] : mDirections)
	{
		size_t newRow = aRow + dy;
		size_t newCol = aCol + dx;

		if (newRow < aLayer->size() && newCol < (*aLayer)[newRow].size() && isWalkableTile((*aLayer)[newRow][newCol]))
		{
			int adjacentNode = calculateNodeIndex(newRow, newCol, (*aLayer)[newRow].size());

			if (uniqueAdjacentNodes.find(adjacentNode) == uniqueAdjacentNodes.end())
			{
				addEdge(aCurrentNode, adjacentNode);
				uniqueAdjacentNodes.insert(adjacentNode);
			}
		}
	}
}

/**
 * @brief Checks if a tile is walkable
 *
 * @param aTileID
 * @return true
 * @return false
 */
bool MapToGraph::isWalkableTile(int tileID) const { return mWalkableTiles.find(tileID) != mWalkableTiles.end(); }

/**
 * @brief Add an edge between two nodes
 *
 * @param aFrom
 * @param aTo
 */
void MapToGraph::addEdge(int aFrom, int aTo)
{
	auto& fromConnections = mAdjacencyList[aFrom];
	auto& toConnections = mAdjacencyList[aTo];

	if (std::find(fromConnections.begin(), fromConnections.end(), aTo) == fromConnections.end())
	{
		fromConnections.push_back(aTo);
	}

	if (std::find(toConnections.begin(), toConnections.end(), aFrom) == toConnections.end())
	{
		toConnections.push_back(aFrom);
	}
}

/**
 * @brief Getter for the adjacency list (The graph)
 *
 * @return const std::unordered_map<int, std::vector<int>>&
 */
const std::unordered_map<int, std::vector<int>>& MapToGraph::getAdjacencyList() const { return mAdjacencyList; }
