#include "TileMapParser.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

/**
 * @brief Construct a new TileMapParser object
 * 
 * @param aFilePath 
 */
TileMapParser::TileMapParser(const std::string& aFilePath) : mFilePath(aFilePath) {}


/**
 * @brief Parses the JSON file
 * 
 * @details Parse() reads the JSON file and calls the private storeTileInfo() and storeObjectLayer() methods accordingly
 */
void TileMapParser::parse()
{
	std::ifstream file(mFilePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file: " + mFilePath);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();

	if (content.empty())
	{
		throw std::runtime_error("File is empty: " + mFilePath);
	}

	try
	{
		mJsonData = nlohmann::json::parse(content);
	}
	catch (const nlohmann::json::parse_error& e)
	{
		throw std::runtime_error("Invalid JSON: " + mFilePath);
	}

	if (mJsonData.is_null())
	{
		throw std::runtime_error("File is empty: " + mFilePath);
	}

	if (!mJsonData.contains("tilesets"))
	{
		throw std::runtime_error("Missing 'tilesets' in JSON: " + mFilePath);
	}

	if (!mJsonData.contains("layers"))
	{
		throw std::runtime_error("Missing 'layers' in JSON: " + mFilePath);
	}

	// Extract tilesets
	for (const auto& tileset : mJsonData["tilesets"])
	{
		mTilesets.push_back(tileset);
	}

	// Extract layers
	for (const auto& layer : mJsonData["layers"])
	{
		if (!layer.contains("type"))
		{
			throw std::runtime_error("Layer missing 'type' key");
		}
		if (layer["type"] == "tilelayer")
		{
			int width = layer["width"];
			int height = layer["height"];
			std::vector<std::vector<int>> grid(height, std::vector<int>(width));

			const auto& data = layer["data"];
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					grid[y][x] = data[y * width + x];
				}
			}
			mTileMapData.mLayers.push_back(grid);
			if (layer.contains("name"))
			{
				mTileMapData.mLayerNames.push_back(layer["name"]);
			}
		}
		else if (layer["type"] == "objectgroup")
		{
			parseObjectLayer(layer);
		}
	}

	// Store tile information in a map
	storeTileInfo();
}

/**
 * @brief Parses the objects of JSON TileMap Object Layer
 * 
 * @param layer 
 */
void TileMapParser::parseObjectLayer(const nlohmann::json& layer)
{
	if (!layer.contains("objects") || !layer["objects"].is_array())
	{
		throw std::runtime_error("Object layer 'objects' is missing or not an array in JSON: " + mFilePath);
	}
	for (const auto& object : layer["objects"])
	{
		MapObject mapObject;
		mapObject.x = object["x"];
		mapObject.y = object["y"];
		mapObject.width = object["width"];
		mapObject.height = object["height"];

		if (object.contains("properties"))
		{
			for (const auto& property : object["properties"])
			{
				mapObject.properties[property["name"]] = property["value"].dump();
			}
		}

		mTileMapData.mMapObjects.push_back(mapObject);
	}
}


/**
 * @brief Getter for a tile position
 * 
 * @param gID 
 * @return std::pair<int, int> 
 */
std::pair<int, int> TileMapParser::getTilePosition(int gID) const
{
	for (const auto& tileset : mTilesets)
	{
		int firstGID = tileset["firstgid"];
		int tileCount = tileset["tilecount"];
		int columns = tileset["columns"];

		if (gID >= firstGID && gID < firstGID + tileCount)
		{
			int localID = gID - firstGID;
			int tileWidth = tileset["tilewidth"];
			int tileHeight = tileset["tileheight"];
			int x = (localID % columns) * tileWidth;
			int y = (localID / columns) * tileHeight;
			return {x, y};
		}
	}
	throw std::runtime_error("gID not found in any tileset");
}


/**
 * @brief Stores tile information in a map into a TileMapData struct
 * 
 */
void TileMapParser::storeTileInfo()
{
	std::unordered_set<int> usedGIDs;

	// Collect all used gIDs from the layers
	if (mTileMapData.mLayers.empty())
	{
		throw std::runtime_error("No layers found in JSON: " + mFilePath);
	}
	for (const auto& layer : mTileMapData.mLayers)
	{
		for (const auto& row : layer)
		{
			for (int gID : row)
			{
				if (gID != 0)
				{ // Assuming 0 is an empty tile
					usedGIDs.insert(gID);
				}
			}
		}
	}

	// Store tile information for the used gIDs
	for (int gID : usedGIDs)
	{
		for (const auto& tileset : mTilesets)
		{
			int firstGID = tileset["firstgid"];
			int tileCount = tileset["tilecount"];
			std::string tilesetName = tileset["image"];

			if (gID >= firstGID && gID < firstGID + tileCount)
			{
				auto position = getTilePosition(gID);
				TileInfo tileInfo = {tilesetName, position, {}};

				// Check for colliders
				int localID = gID - firstGID;
				if (tileset.contains("tiles"))
				{
					for (const auto& tile : tileset["tiles"])
					{
						if (tile["id"] == localID && tile.contains("objectgroup"))
						{
							for (const auto& object : tile["objectgroup"]["objects"])
							{
								try
								{
									ColliderData collider = {object.at("x").get<float>(), object.at("y").get<float>(),
															 object.at("width").get<float>(),
															 object.at("height").get<float>()};
									tileInfo.mColliders.push_back(collider);
								}
								catch (const nlohmann::json::type_error& e)
								{
									std::cerr << "Error parsing collider for gID " << gID << ": " << e.what()
											  << std::endl;
								}
							}
						}
					}
				}

				mTileMapData.mTileInfoMap[gID] = tileInfo;
				break;
			}
		}
	}
}


/**
 * @brief Getter for the parsed TileMapData
 * 
 * @return const TileMapData& 
 */
const TileMapData& TileMapParser::getTileMapData() const { return mTileMapData; }