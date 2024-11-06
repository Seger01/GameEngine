#ifndef TILEMAPPARSER_H
#define TILEMAPPARSER_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

struct ColliderData {
    float x;
    float y;
    float mWidth;
    float mHeight;
};

struct TileInfo {
    std::string mTilesetName;
    std::pair<int, int> mCoordinates;
    std::vector<ColliderData> mColliders;
};

struct SpawnPoint {
    float x;
    float y;
    bool isPlayerSpawn;
};

struct TileMapData {
    std::vector<std::vector<std::vector<int>>> mLayers;
    std::unordered_map<int, TileInfo> mTileInfoMap;
    std::vector<SpawnPoint> mSpawnPoints;
};

class TileMapParser {
public:
    TileMapParser(const std::string& aFilePath);
    //virtual ~TileMapParser();
    void parse();
    std::pair<int, int> getTilePosition(int gID) const;
    int getGIDFromCoordinate(int layer, int x, int y) const;
    void storeTileInfo();
    void parseObjectLayer(const nlohmann::json& layer);
    void printTileInfo(int gID) const;
    void printLayers() const;
    void printTileInfoMap() const;
    std::pair<int, int> getGridTilePosition(int layerIndex, int x, int y) const;
    const TileMapData& getTileMapData() const;

private:
    std::string mFilePath;
    nlohmann::json mJsonData;
    std::vector<nlohmann::json> mTilesets;
    std::vector<std::string> mLayerNames;
    TileMapData mTileMapData;
};

#endif // TILEMAPPARSER_H