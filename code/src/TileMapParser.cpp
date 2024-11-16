#include "TileMapParser.h"

TileMapParser::TileMapParser(const std::string& aFilePath) : mFilePath(aFilePath) {}

void TileMapParser::parse() {
    std::ifstream file(mFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + mFilePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    if (content.empty()) {
        throw std::runtime_error("File is empty: " + mFilePath);
    }

    try {
        mJsonData = nlohmann::json::parse(content);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("File is empty or invalid JSON: " + mFilePath);
    }

    if (mJsonData.is_null()) {
        throw std::runtime_error("File is empty: " + mFilePath);
    }

    if (!mJsonData.contains("tilesets")) {
        throw std::runtime_error("Missing 'tilesets' in JSON: " + mFilePath);
    }

    if (!mJsonData.contains("layers")) {
        throw std::runtime_error("Missing 'layers' in JSON: " + mFilePath);
    }
    
    // Extract tilesets
    for (const auto& tileset : mJsonData["tilesets"]) {
        mTilesets.push_back(tileset);
    }

    // Extract layers
    for (const auto& layer : mJsonData["layers"]) {
        if (layer["type"] == "tilelayer") {
            int width = layer["width"];
            int height = layer["height"];
            std::vector<std::vector<int>> grid(height, std::vector<int>(width));

            const auto& data = layer["data"];
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    grid[y][x] = data[y * width + x];
                }
            }
            mTileMapData.mLayers.push_back(grid);
            mTileMapData.mLayerNames.push_back(layer["name"]);
        }
        else if (layer["type"] == "objectgroup") {
            parseObjectLayer(layer);
        }
    }

    // Store tile information in a map
    storeTileInfo();
}

void TileMapParser::parseObjectLayer(const nlohmann::json& layer) {
    for (const auto& object : layer["objects"]) {
        if (object.contains("properties")) {
            for (const auto& property : object["properties"]) {
                if (property["name"] == "isPlayerSpawn" && property["type"] == "bool" && property["value"] == true) {
                    SpawnPoint spawnPoint;
                    spawnPoint.x = object["x"];
                    spawnPoint.y = object["y"];
                    spawnPoint.width = object["width"];
                    spawnPoint.height = object["height"];
                    spawnPoint.isPlayerSpawn = true;
                    mTileMapData.mSpawnPoints.push_back(spawnPoint);
                } else if (property["name"] == "isEnemySpawn" && property["type"] == "bool" && property["value"] == true) {
                    SpawnPoint spawnPoint;
                    spawnPoint.x = object["x"];
                    spawnPoint.y = object["y"];
                    spawnPoint.width = object["width"];
                    spawnPoint.height = object["height"];
                    spawnPoint.isEnemySpawn = true;
                    for (const auto& prop : object["properties"]) {
                        if (prop["name"] == "roomID" && prop["type"] == "string") {
                            spawnPoint.roomID = prop["value"];
                        }
                    }
                    mTileMapData.mSpawnPoints.push_back(spawnPoint);
                } else if (property["name"] == "roomID" && property["type"] == "string") {
                    RoomTrigger roomTrigger;
                    roomTrigger.x = object["x"];
                    roomTrigger.y = object["y"];
                    roomTrigger.mWidth = object["width"];
                    roomTrigger.mHeight = object["height"];
                    roomTrigger.roomID = property["value"];
                    if (object["type"] == "room_entry") {
                        mTileMapData.mRoomTriggers.push_back(roomTrigger);
                    }
                }
            }
        }
    }
}

std::pair<int, int> TileMapParser::getTilePosition(int gID) const {
    for (const auto& tileset : mTilesets) {
        int firstGID = tileset["firstgid"];
        int tileCount = tileset["tilecount"];
        int columns = tileset["columns"];

        if (gID >= firstGID && gID < firstGID + tileCount) {
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

void TileMapParser::storeTileInfo() {
    std::unordered_set<int> usedGIDs;

    // Collect all used gIDs from the layers
    for (const auto& layer : mTileMapData.mLayers) {
        for (const auto& row : layer) {
            for (int gID : row) {
                if (gID != 0) { // Assuming 0 is an empty tile
                    usedGIDs.insert(gID);
                }
            }
        }
    }

    // Store tile information for the used gIDs
    for (int gID : usedGIDs) {
        for (const auto& tileset : mTilesets) {
            int firstGID = tileset["firstgid"];
            int tileCount = tileset["tilecount"];
            std::string tilesetName = tileset["image"];

            if (gID >= firstGID && gID < firstGID + tileCount) {
                auto position = getTilePosition(gID);
                TileInfo tileInfo = {tilesetName, position, {}};

                // Check for colliders
                int localID = gID - firstGID;
                if (tileset.contains("tiles")) {
                    for (const auto& tile : tileset["tiles"]) {
                        if (tile["id"] == localID && tile.contains("objectgroup")) {
                            for (const auto& object : tile["objectgroup"]["objects"]) {
                                try {
                                    ColliderData collider = {
                                        object.at("x").get<float>(),
                                        object.at("y").get<float>(),
                                        object.at("width").get<float>(),
                                        object.at("height").get<float>()
                                    };
                                    tileInfo.mColliders.push_back(collider);
                                } catch (const nlohmann::json::type_error& e) {
                                    std::cerr << "Error parsing collider for gID " << gID << ": " << e.what() << std::endl;
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

const TileMapData& TileMapParser::getTileMapData() const {
    return mTileMapData;
}