/**
 * @file FloatSaveField.h
 *
 * Contains the declaration of the FloatSaveField class.
 */
#pragma once

#include <string>

/**
 * @class FloatSaveField
 *
 * @brief A class representing a float field in a save game. Contains a name and a value.
 */
class FloatSaveField
{
public:
	FloatSaveField(const std::string& aName, float aValue = 0);

	std::string getName() const;
	void setValue(float aValue);
	float getValue() const;

private:
	std::string mName;
	float mValue;
};