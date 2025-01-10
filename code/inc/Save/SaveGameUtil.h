/**
 * @file SaveGameUtil.h
 *
 * Contains the declaration of the SaveGameUtil class.
 */
#pragma once

#include <string>

/**
 * @class SaveGameUtil
 *
 * @brief A utility class for save game operations.
 */
class SaveGameUtil
{
public:
	static bool isInteger(const std::string& aValue);
	static bool isFloat(const std::string& aValue);
};