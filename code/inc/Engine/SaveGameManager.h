#pragma once
#include "SaveGame.h"
#include <string>
#include <unordered_map>
#include <vector>

class SaveGameManager {
public:
    friend class EngineBravo;
    SaveGame& createSaveGame(const std::string& aId, const std::string& aPath);
    SaveGame& getSaveGame(const std::string& aId);
    void deleteSaveGame(const std::string& aId, const bool aDeleteFile = false);

private:
    SaveGameManager() = default;

private:
    std::unordered_map<std::string, SaveGame> mSaveGames;
};