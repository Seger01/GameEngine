/**
 * @file SaveGameManager.h
 *
 * Contains the declaration of the SaveGameManager class.
 */
#pragma once

#include "SaveGame.h"

#include <string>
#include <unordered_map>

/**
 * @class SaveGameManager
 *
 * @brief A class representing a save game manager. Contains a map of save games.
 */
class SaveGameManager
{
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