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
 *
 * The savegame system is largely similar to its equivalent in Unity.
 * The three data types which can be stored are represented by separate classes (IntSaveField, FloatSaveField and
 * StringSaveField). This way, the user does never need to manually check for types before retrieving the data.
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