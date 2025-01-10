/**
 * @file TileMapParser.cpp
 * @brief This file contains the implementation of the TileMapParser class
 */
#include "TileMapParser.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

TileMapParser::TileMapParser(const std::string& aFilePath) : mFilePath(aFilePath) {}

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
				std::string layerName = layer["name"];
				mTileMapData.mLayerNames.push_back(layerName);
				parseLayerProperties(layer, layerName);
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

void TileMapParser::parseLayerProperties(const nlohmann::json& layer, const std::string& layerName)
{
	if (layer.contains("properties"))
	{
		std::unordered_map<std::string, std::string> properties;
		for (const auto& property : layer["properties"])
		{
			std::string name = property["name"];
			std::string value;
			if (property["type"] == "bool")
			{
				value = property["value"].get<bool>() ? "true" : "false";
			}
			else if (property["type"] == "int")
			{
				value = std::to_string(property["value"].get<int>());
			}
			else if (property["type"] == "float")
			{
				value = std::to_string(property["value"].get<float>());
			}
			else if (property["type"] == "string")
			{
				value = property["value"].get<std::string>();
			}
			else
			{
				value = property["value"].dump(); // Fallback to JSON string representation
			}
			properties[name] = value;
		}
		mTileMapData.mLayerProperties[layerName] = properties;
	}
}

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
		mapObject.type = object.value("type", "");
		mapObject.name = object.value("name", "");

		if (object.contains("properties"))
		{
			for (const auto& property : object["properties"])
			{
				std::string propertyName = property["name"];
				if (property["type"] == "bool")
				{
					mapObject.properties[propertyName] = property["value"].get<bool>() ? "true" : "false";
				}
				else if (property["type"] == "int")
				{
					mapObject.properties[propertyName] = std::to_string(property["value"].get<int>());
				}
				else if (property["type"] == "float")
				{
					mapObject.properties[propertyName] = std::to_string(property["value"].get<float>());
				}
				else if (property["type"] == "string")
				{
					mapObject.properties[propertyName] = property["value"].get<std::string>();
				}
				else
				{
					throw std::runtime_error("Unhandled property type: " + property["type"].get<std::string>());
				}
			}
		}
		mTileMapData.mMapObjects.push_back(mapObject);
	}
}

std::pair<int, int> TileMapParser::getTilePosition(int gID) const
{
	for (const auto& tileset : mTilesets)
	{
		int firstGID = tileset["firstgid"];
		int tileCount = tileset["tilecount"];

		if (gID >= firstGID && gID < firstGID + tileCount)
		{
			int columns = tileset["columns"];
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

void TileMapParser::storeTileInfo()
{
	std::unordered_set<int> usedGIDs;

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
				{
					usedGIDs.insert(gID);
				}
			}
		}
	}

	for (int gID : usedGIDs)
	{
		for (const auto& tileset : mTilesets)
		{
			int firstGID = tileset["firstgid"];
			int tileCount = tileset["tilecount"];

			if (gID >= firstGID && gID < firstGID + tileCount)
			{
				std::string tilesetName = tileset["image"];
				auto position = getTilePosition(gID);
				TileInfo tileInfo = {tilesetName, position, {}};

				if (tileset.contains("tiles"))
				{
					int localID = gID - firstGID;

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
									throw std::runtime_error(
										"Error parsing collider for gID " + std::to_string(gID) + ": " + e.what());
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

const TileMapData& TileMapParser::getTileMapData() const { return mTileMapData; }