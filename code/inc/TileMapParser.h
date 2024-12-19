/**
 * @file TileMapParser.h
 * @brief This file contains the declaration of the TileMapParser class and the structs used to store the parsed data.
 * @details Tilemaps are a way to make a level made of small, resuable tiles and thus create a 2D level in an easy way.
	Tilemaps will be parsed in the engine using the TileMapParser class.

	The map creator that is used to create the tilemaps for the engine is Tiled.
	Tiled is a free, open-source third party map editor that can be used to create 2D maps.
	The reason Tiled is chosen is primarily because it was recommended for this project.
	Another advantage Tiled has is its extensive documentation and its ease of use.
	Maps created in Tiled are exported in the JSON format.
	The engine will parse the map file using a library called nlohmann/json, which is a popular libary for parsing JSON files.
	From the parsed map, the LevelBuilder class can be used to build a scene and load the tiles.
	Also, a graph can be created form a layer which has the custom property "isGraphLayer" set to true using the MapToGraph class.
 */
#pragma once

#include <string>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Struct to store data of colliders
 * 
 */
struct ColliderData {
    float x;
    float y;
    float mWidth;
    float mHeight;
};

/**
 * @brief Struct to store tile information
 * 
 */
struct TileInfo {
    std::string mTilesetName;
    std::pair<int, int> mCoordinates;
    std::vector<ColliderData> mColliders;
};


/**
 * @brief Struct to store map object data, like triggers
 * 
 */
struct MapObject {
    float x;
    float y;
    float width;
    float height;
    std::string type;
    std::string name;
    std::unordered_map<std::string, std::string> properties;
};


/**
 * @brief Main Struct to store a parsed map
 * 
 */
struct TileMapData {
    std::vector<std::vector<std::vector<int>>> mLayers;
    std::vector<std::string> mLayerNames;
    std::unordered_map<int, TileInfo> mTileInfoMap;
    std::vector<MapObject> mMapObjects;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mLayerProperties;
};


/**
 * @brief Class to convert a JSON TileMap to a TileMapData struct
 * 
 * @details The class reads a JSON file and converts it to a TileMapData struct using the nlohmann JSON library
 *          Use parse to convert the JSON file to the TileMapData struct
 * @param aFilePath The path to the JSON file
 */
class TileMapParser {
public:
    TileMapParser(const std::string& aFilePath);
    void parse();
    std::pair<int, int> getTilePosition(int gID) const;
    const TileMapData& getTileMapData() const;

private:
    void storeTileInfo();
    void parseObjectLayer(const nlohmann::json& layer);
    void parseLayerProperties(const nlohmann::json& layer, const std::string& layerName); 
private:
    /// @brief The path to the JSON file
    std::string mFilePath;

    /// @brief The JSON data
    nlohmann::json mJsonData;

    /// @brief The tilesets
    std::vector<nlohmann::json> mTilesets;

    /// @brief The parsed tilemap data
    TileMapData mTileMapData;
};