#pragma once

#include "SaveGame.hpp"
#include <string>
#include <unordered_map>

class SaveGameManager {
public:
  SaveGame &createSaveGame(const std::string &aId, const std::string &aPath);
  void deleteSaveGame(const std::string &aId);

private:
  std::unordered_map<std::string, SaveGame> mSaveGames;
};