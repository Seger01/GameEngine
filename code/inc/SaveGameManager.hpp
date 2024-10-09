#pragma once

#include "SaveGame.hpp"
#include <string>
#include <unordered_map>

class SaveGameManager {
public:
  SaveGame &createSaveGame(const std::string &aId, const std::string &aPath);
  SaveGame &getSaveGame(const std::string &aId);
  void deleteSaveGame(const std::string &aId, const bool aDeleteFile = false);

private:
  std::unordered_map<std::string, SaveGame> mSaveGames;
};