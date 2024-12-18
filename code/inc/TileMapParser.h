/**
 * @file TileMapParser.h
 * @brief This file contains the declaration of the TileMapParser class and the structs used to store the parsed data
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