/**
 * @file IntSaveField.h
 *
 * Contains the declaration of the IntSaveField class.
 */
#pragma once

#include <string>

/**
 * @class IntSaveField
 *
 * @brief A class representing an integer field in a save game. Contains a name and a value.
 */
class IntSaveField
{
public:
	IntSaveField(const std::string& aName, int aValue = 0);

	std::string getName() const;
	void setValue(int aValue);
	int getValue() const;

private:
	std::string mName;
	int mValue;
};