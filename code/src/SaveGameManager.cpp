#include "SaveGameManager.hpp"

SaveGame &SaveGameManager::createSaveGame(const std::string &aId,
                                          const std::string &aPath) {
  SaveGame saveGame{aPath};
  mSaveGames[aId] = saveGame;
  return mSaveGames[aId];
}

void SaveGameManager::deleteSaveGame(const std::string &aId) {
  mSaveGames.erase(aId);
}