/**
 * @file SaveGameManager.cpp
 *
 * Contains the implementation of the SaveGameManager class.
 */
#include "Engine/SaveGameManager.h"

#include <stdexcept>

/**
 * @brief Create a new save game with the specified ID and path.
 *
 * @param aId The ID of the save game.
 * @param aPath The path to the save game file.
 */
SaveGame& SaveGameManager::createSaveGame(const std::string& aId, const std::string& aPath)
{
	auto result = mSaveGames.emplace(std::piecewise_construct,
									 std::forward_as_tuple(aId),  // Forwards aId to the key constructor
									 std::forward_as_tuple(aPath) // Forwards aPath to the value constructor
	);
	return result.first->second;
}

/**
 * @brief Get a save game by its ID.
 *
 * @param aId The ID of the save game.
 *
 * @return The save game.
 *
 * @throw std::invalid_argument if the save game does not exist.
 */
SaveGame& SaveGameManager::getSaveGame(const std::string& aId)
{
	auto it = mSaveGames.find(aId);
	if (it == mSaveGames.end())
	{
		throw std::invalid_argument("Save game with ID " + aId + " does not exist.");
	}
	return it->second;
}

/**
 * @brief Delete a save game by its ID.
 *
 * @param aId The ID of the save game.
 * @param aDeleteFile Whether to delete the save game file. Defaults to false
 *
 * @throw std::invalid_argument if the save game does not exist.
 */
void SaveGameManager::deleteSaveGame(const std::string& aId, const bool aDeleteFile)
{
	auto it = mSaveGames.find(aId);
	if (it == mSaveGames.end())
	{
		throw std::invalid_argument("Save game with ID " + aId + " does not exist.");
	}
	if (aDeleteFile)
	{
		it->second.remove();
	}
	mSaveGames.erase(it);
}
