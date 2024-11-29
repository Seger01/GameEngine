#include "FSConverter.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdexcept>
#include <algorithm>

std::string FSConverter::mCachedResourceDir;

/**
 * @brief Construct a new FSConverter::FSConverter object   
 * @details takes an optional string containing the path to the Resources directory.
 * @param aResourceDir 
 */
FSConverter::FSConverter(std::string aResourceDir)
{
	if (!mCachedResourceDir.empty())
	{
		mResourceDir = mCachedResourceDir;
	}
	else
	{
		mResourceDir = aResourceDir;
		if (mResourceDir.empty())
		{
			mResourceDir = findResourcesFolder();
		}

		if (mResourceDir.empty())
		{
			throw std::runtime_error("Resources folder not found.");
		}
		else
		{
			mCachedResourceDir = mResourceDir;
		}
	}
}


/**
 * @brief Finds the Resources folder
 * 
 * @return std::string 
 */
std::string FSConverter::findResourcesFolder() const
{
	std::filesystem::path execPath = executablePath();

	// Try to find the Resources folder in a few places relative to the executable
	std::filesystem::path potentialPaths[] = {
		execPath / "Resources",
		execPath / "../Resources",
		execPath / "../../Resources",
		execPath / "../../../Resources"
	};

    auto it = std::find_if(std::begin(potentialPaths), std::end(potentialPaths), [](const std::filesystem::path& path) 
    {
        return std::filesystem::exists(path);
    });

    if (it != std::end(potentialPaths))
	{
        return it->string();
    }

	throw std::runtime_error("Resources folder not found in any of the checked paths.");
	return ""; // Resources folder not found
}


/**
 * @brief Finds the resource path
 * 
 * @param resourceName 
 * @param aCheckExists 
 * @return std::string 
 */
std::string FSConverter::getResourcePath(const std::string& resourceName, bool aCheckExists) const
{
	std::filesystem::path fullPath = std::filesystem::path(mResourceDir) / resourceName;

	if (aCheckExists && !std::filesystem::exists(fullPath))
	{
        throw std::runtime_error("Error: Resource " + resourceName + " does not exist at " + fullPath.string());
		return "";
	}

	return fullPath.string();
}


/**
 * @brief Finds the executable path
 * 
 * @return std::string 
 */
std::string FSConverter::executablePath() const
{
	char path[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
	if (count == -1)
	{
		throw std::runtime_error("Unable to determine the executable path.");
	}
	return std::filesystem::path(path).parent_path().string();
}
