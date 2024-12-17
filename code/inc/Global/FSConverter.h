/**
 * @file FSConverter.h
 * @brief This file contains the declaration of the FSConverter class
 */
#pragma once

#include <filesystem>
#include <limits.h>
#include <stdexcept>
#include <string>
#include <unistd.h>

/**
 * @brief Class to direct a file path to the Resources directory
 * @details
 * This class is responsible for converting a file path to the Resources directory.
 * The main method to perform the conversion is getResourcePath().
 * Once the constructor for the class is called, the Resources directory is cached, and wont have to be found again.
 * @param aResourceDir An optional string containing the path to the Resources directory.
 */
class FSConverter
{
public:
	FSConverter(const std::string& aResourceDir = "");

	std::string getResourcePath(const std::string& resourceName, bool aCheckExists = false) const;

private:
	std::string findResourcesFolder() const;
	std::string executablePath() const;

private:
	/// @brief The path to the Resources directory.
	std::string mResourceDir;

	/// @brief Static path to the Resources directory.
	static std::string mCachedResourceDir;
};
