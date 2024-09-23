#include "SaveGameTest.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

SaveGameTest::SaveGameTest(std::string path) : mPath(path) {
  std::ifstream inputFile{mPath};
  if (!inputFile.is_open()) {
    throw std::runtime_error("Unable to open json file");
  }

  inputFile >> jsonData;

  inputFile.close();

  setLevelNr(jsonData["levelNr"]);
  setLevelStatus(jsonData["levelStatus"]);
  setPlayerLevel(jsonData["playerLevel"]);
  std::vector<unsigned> temp;
  for (const auto &item : jsonData["inventory"]) {
    temp.push_back(item);
  }
  setInventory(temp);
}

void SaveGameTest::write() {
  jsonData["levelNr"] = getLevelNr();
  jsonData["levelStatus"] = getLevelStatus();
  jsonData["playerLevel"] = getPlayerLevel();
  jsonData["inventory"] = getInventory();
  // Create an output file stream
  std::ofstream outputFile(mPath);

  // Check if the file was opened successfully
  if (!outputFile.is_open()) {
    throw std::runtime_error("Unable to open the JSON file for writing.");
  }

  // Write the JSON object to the file
  outputFile << jsonData.dump(4); // Pretty print with 4 spaces indentation

  // Close the file
  outputFile.close();
}

void SaveGameTest::setLevelNr(int value) { levelNr = value; }
void SaveGameTest::setLevelStatus(std::string value) { levelStatus = value; }
void SaveGameTest::setPlayerLevel(int value) { playerLevel = value; }
void SaveGameTest::setInventory(std::vector<unsigned> &value) {
  inventory = value;
}

int SaveGameTest::getLevelNr() const { return levelNr; }
std::string SaveGameTest::getLevelStatus() const { return levelStatus; }
int SaveGameTest::getPlayerLevel() const { return playerLevel; }
std::vector<unsigned> SaveGameTest::getInventory() const { return inventory; }