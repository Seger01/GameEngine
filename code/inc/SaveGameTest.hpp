#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class SaveGameTest {
public:
  SaveGameTest(std::string path);
  void readFile();
  void write();

public: // Setters
  void setLevelNr(int);
  void setLevelStatus(std::string);
  void setPlayerLevel(int);
  void setInventory(std::vector<unsigned> &);

public: // Getters
  int getLevelNr() const;
  std::string getLevelStatus() const;
  int getPlayerLevel() const;
  std::vector<unsigned> getInventory() const;

private:
  std::string mPath;
  nlohmann::json jsonData;
  int levelNr;
  std::string levelStatus;
  int playerLevel;
  std::vector<unsigned> inventory;
};