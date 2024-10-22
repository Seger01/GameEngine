#pragma once
#include <string>
#include <vector>
#include "SaveGame.h"

class SaveGameManager {
    public:
        friend class EngineBravo;
        SaveGameManager();
        ~SaveGameManager();
        SaveGame &createSaveGame(const std::string &aId, const std::string &aPath);
        SaveGame &getSaveGame(const std::string &aId);
        void deleteSaveGame(const std::string &aId, const bool aDeleteFile = false);
    private:
        std::vector<SaveGame> mSaveGames;
};