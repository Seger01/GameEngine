#include "SaveGame.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

SaveGame::SaveGame(std::string path) {
    std::ifstream inputFile {path};
    if (!inputFile.is_open()) {
        throw std::runtime_error("Unable to open json file");
    }

    inputFile >> jsonData;

    inputFile.close();

    setLevelNr(jsonData["levelNr"]);
    setLevelStatus(jsonData["levelStatus"]);
    setPlayerLevel(jsonData["playerLevel"]);
    std::vector<unsigned> temp;
    for (const auto& item : jsonData["inventory"]) {
        temp.push_back(item);
    }
    setInventory(temp);
}

void SaveGame::setLevelNr(int value) {
    levelNr = value;
}
void SaveGame::setLevelStatus(std::string value) {
    levelStatus = value;
}
void SaveGame::setPlayerLevel(int value) {
    playerLevel = value;
}
void SaveGame::setInventory(std::vector<unsigned>& value) {
    inventory = value;
}

int SaveGame::getLevelNr() const {
    return levelNr;
}
std::string SaveGame::getLevelStatus() const {
    return levelStatus;
}
int SaveGame::getPlayerLevel() const {
    return playerLevel;
}
std::vector<unsigned> SaveGame::getInventory() const {
    return inventory;
}