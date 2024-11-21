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
    float width;
    float height;
    bool isPlayerSpawn = false;
    bool isEnemySpawn = false;
    std::string roomID = "";
};

struct RoomTrigger {
    float x;
    float y;
    float mWidth;
    float mHeight;
    std::string roomID;
};

struct LevelEndTrigger {
    float x;
    float y;
    float mWidth;
    float mHeight;
    std::string nextLevel;
};

struct TileMapData {
    std::vector<std::vector<std::vector<int>>> mLayers;
    std::vector<std::string> mLayerNames;
    std::unordered_map<int, TileInfo> mTileInfoMap;
    std::vector<SpawnPoint> mSpawnPoints;
    std::vector<RoomTrigger> mRoomTriggers;
    std::vector<LevelEndTrigger> mLevelEndTriggers;
};

class TileMapParser {
public:
    TileMapParser(const std::string& aFilePath);
    void parse();
    std::pair<int, int> getTilePosition(int gID) const;
    void storeTileInfo();
    void parseObjectLayer(const nlohmann::json& layer);
    const TileMapData& getTileMapData() const;

private:
    std::string mFilePath;
    nlohmann::json mJsonData;
    std::vector<nlohmann::json> mTilesets;
    TileMapData mTileMapData;
};

#endif // TILEMAPPARSER_H