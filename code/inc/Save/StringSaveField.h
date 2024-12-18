/**
 * @file StringSaveField.h
 *
 * Contains the declaration of the StringSaveField class.
 */
#pragma once

#include <string>

/**
 * @class StringSaveField
 *
 * @brief A class representing a string field in a save game. Contains a name and a value.
 */
class StringSaveField
{
public:
	StringSaveField(const std::string& aName, const std::string& aValue = "");

	std::string getName() const;
	void setValue(const std::string& aValue);
	std::string getValue() const;

private:
	/// @brief The name of the field. Used to identify it.
	std::string mName;
	/// @brief The value of the field.
	std::string mValue;
};