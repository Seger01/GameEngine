#include "MapToGraph.h"
#include <iostream>

MapToGraph::MapToGraph(const TileMapData& aTileMapData)
	: mTileMapData(aTileMapData), mDirections{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}
{
	preprocessWalkableTiles();
}


// Preprocess walkable tiles
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


// Find the Graph layer
const std::vector<std::vector<int>>* MapToGraph::findGraphLayer() const
{
	for (size_t i = 0; i < mTileMapData.mLayers.size(); ++i)
	{
		if (mTileMapData.mLayerNames[i] == "Graph")std::cerr << "Floor layer not found!" << std::endl;
		{
			return &mTileMapData.mLayers[i];
		}
	}
	return nullptr;
}


// Convert tilemap to graph
void MapToGraph::convertToGraph()
{
	const auto* graphLayer = findGraphLayer();
	if (!graphLayer)
	{
		std::runtime_error("Graph layer not found in map! (MapToGraph::convertToGraph)");
		return;
	}

	for (size_t row = 0; row < graphLayer->size(); ++row)
	{
		for (size_t col = 0; col < (*graphLayer)[row].size(); ++col)
		{
			int currentTileID = (*graphLayer)[row][col];
			int currentNode = calculateNodeIndex(row, col, graphLayer->at(row).size());
			connectAdjacentNodes(currentNode, row, col, *graphLayer);
		}
	}
}


// Calculate unique node index
int MapToGraph::calculateNodeIndex(size_t aRow, size_t aCol, size_t aWidth) const { return aRow * aWidth + aCol; }


// Connect adjacent walkable nodes
void MapToGraph::connectAdjacentNodes(int aCurrentNode, size_t aRow, size_t aCol,
									  const std::vector<std::vector<int>>& aLayer)
{
	for (const auto& [dx, dy] : mDirections)
	{
		size_t newRow = aRow + dx;
		size_t newCol = aCol + dy;

		// Bounds and walkability check
		if (newRow < aLayer.size() && newCol < aLayer[newRow].size() && isWalkableTile(aLayer[newRow][newCol]))
		{
			int adjacentNode = calculateNodeIndex(newRow, newCol, aLayer[newRow].size());
			addEdge(aCurrentNode, adjacentNode);
		}
	}
}


// Check if a tile is walkable
bool MapToGraph::isWalkableTile(int aTileID) const { return aTileID != 0 && mWalkableTiles.count(aTileID) > 0; }


// Add an edge between nodes
void MapToGraph::addEdge(int aFrom, int aTo)
{
	mAdjacencyList[aFrom].push_back(aTo);
	mAdjacencyList[aTo].push_back(aFrom);
}


// Get the adjacency list
const std::unordered_map<int, std::vector<int>>& MapToGraph::getAdjacencyList() const { return mAdjacencyList; }


// Print the graph for debugging
void MapToGraph::printGraph() const
{
	for (const auto& pair : mAdjacencyList)
	{
		std::cout << "Node " << pair.first << ": ";
		for (int neighbor : pair.second)
		{
			std::cout << neighbor << " ";
		}
		std::cout << std::endl;
	}
}