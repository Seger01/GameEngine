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
		if (mTileMapData.mLayerNames[i] == "Graph")std::cerr << "Floor layer not found!" << std::endl;
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


/**
 * @brief Checks if a tile is walkable
 * 
 * @param aTileID 
 * @return true 
 * @return false 
 */
bool MapToGraph::isWalkableTile(int aTileID) const { return aTileID != 0 && mWalkableTiles.count(aTileID) > 0; }


/**
 * @brief Add an edge between two nodes
 * 
 * @param aFrom 
 * @param aTo 
 */
void MapToGraph::addEdge(int aFrom, int aTo)
{
	mAdjacencyList[aFrom].push_back(aTo);
	mAdjacencyList[aTo].push_back(aFrom);
}


/**
 * @brief Getter for the adjacency list (The graph)
 * 
 * @return const std::unordered_map<int, std::vector<int>>& 
 */
const std::unordered_map<int, std::vector<int>>& MapToGraph::getAdjacencyList() const { return mAdjacencyList; }


/**
 * @brief Prints the graph
 * 
 */
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