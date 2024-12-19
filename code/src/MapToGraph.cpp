/**
 * @file MapToGraph.cpp
 * @brief This file contains the implementation of the MapToGraph class.
 */

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
        // Check if the layer has custom properties and if isGraphLayer is set
        auto layerPropertiesIt = mTileMapData.mLayerProperties.find(mTileMapData.mLayerNames[i]);
        if (layerPropertiesIt != mTileMapData.mLayerProperties.end()) {
            auto propertyIt = layerPropertiesIt->second.find("isGraphLayer");
            if (propertyIt != layerPropertiesIt->second.end()) {
				return &mTileMapData.mLayers[i];
            }
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
			if ((*graphLayer)[row][col] != 0)
			{
				int currentNode = calculateNodeIndex(row, col, (*graphLayer)[row].size());
				connectAdjacentNodes(currentNode, row, col, graphLayer);
			}
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
 * @brief Connects adjacent non-zero nodes
 *
 * @param aCurrentNode
 * @param aRow
 * @param aCol
 * @param aLayer
 */
void MapToGraph::connectAdjacentNodes(int aCurrentNode, size_t aRow, size_t aCol,
									  const std::vector<std::vector<int>>* aLayer)
{
	for (const auto& [dx, dy] : mDirections)
	{
		// Calculate newRow and newCol
		int newRow = static_cast<int>(aRow) + dy;
		int newCol = static_cast<int>(aCol) + dx;

		// Ensure the new position is within bounds
		if (newRow < 0 || newRow >= static_cast<int>(aLayer->size()) || newCol < 0 ||
			newCol >= static_cast<int>((*aLayer)[newRow].size()))
		{
			continue;
		}

		// Check if the adjacent tile is non-zero
		if ((*aLayer)[newRow][newCol] != 0)
		{
			int adjacentNode = calculateNodeIndex(newRow, newCol, (*aLayer)[newRow].size());
			addEdge(aCurrentNode, adjacentNode);
		}
	}
}

/**
 * @brief Add an edge between two nodes
 *
 * @param aFrom
 * @param aTo
 */
void MapToGraph::addEdge(int aFrom, int aTo)
{
	// Ensure we have entries for both nodes
	auto& fromConnections = mAdjacencyList[aFrom];
	auto& toConnections = mAdjacencyList[aTo];

	// Add edge only if it doesn't already exist
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