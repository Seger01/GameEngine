#include "SaveGameManager.h"
#include <stdexcept>

SaveGame& SaveGameManager::createSaveGame(const std::string& aId, const std::string& aPath)
{
	auto result = mSaveGames.emplace(std::piecewise_construct,
									 std::forward_as_tuple(aId),  // Forwards aId to the key constructor
									 std::forward_as_tuple(aPath) // Forwards aPath to the value constructor
	);
	return result.first->second;
}

SaveGame& SaveGameManager::getSaveGame(const std::string& aId)
{
	auto it = mSaveGames.find(aId);
	if (it == mSaveGames.end())
	{
		throw std::invalid_argument("Save game with ID " + aId + " does not exist.");
	}
	return it->second;
}

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