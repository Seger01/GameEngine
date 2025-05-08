#include "Save/SaveArray.h"
#include "Save/FloatSaveField.h"
#include "Save/IntSaveField.h"
#include "Save/SaveGame.h"
#include "Save/SaveGameUtil.h"
#include "Save/StringSaveField.h"
#include <algorithm>

#include <iostream>

/**
 * @brief Constructor to initialize the SaveArray with a name.
 *
 * @param aName The name associated with the SaveArray.
 */
SaveArray::SaveArray(const std::string& aName) : mName(aName) {}

/**
 * @brief Gets the name associated with the SaveArray.
 *
 * @return The name as a string.
 */
std::string SaveArray::getName() const { return mName; }

/**
 * @brief Adds a field to the SaveArray. The field type is determined by the type of the value.
 *
 * @param aName The name of the field.
 * @param aValue The value of the field.
 *
 * @throw std::invalid_argument if the value is not an integer, float, or string.
 */
void SaveArray::addAny(const std::string& aName, const nlohmann::json& aValue)
{
	if (aValue.is_number_integer())
	{
		int int_value = aValue.get<int>();
		addIntField(aName, int_value);
	}
	else if (aValue.is_number_float())
	{
		float float_value = aValue.get<float>();
		addFloatField(aName, float_value);
	}
	else if (aValue.is_string())
	{
		std::string str_value = aValue.get<std::string>();
		addStringField(aName, str_value);
	}
	else
	{
		throw std::invalid_argument("Failed to add field with name \"" + aName + "\". Invalid type.");
	}
}

/**
 * @brief Adds an integer field to the SaveArray. If a field with the same name already exists, the value is
 * overwritten.
 *
 * @param aName The name of the field.
 * @param aValue The value of the field.
 */
void SaveArray::addIntField(const std::string& aName, int aValue)
{
	// Check if the field already exists
	auto it = std::find_if(mIntFields.begin(), mIntFields.end(),
						   [&aName](const IntSaveField& field) { return field.getName() == aName; });
	if (it != mIntFields.end())
	{
		std::cout << "SaveArray::addIntField(): cannot add field with name \"" << it->getName()
				  << "\", field already exists. Overwriting value instead" << std::endl;
		it->setValue(aValue);
		return;
	}

	mIntFields.emplace_back(aName, aValue);
}

/**
 * @brief Adds a float field to the SaveArray. If a field with the same name already exists, the value is overwritten.
 *
 * @param aName The name of the field.
 * @param aValue The value of the field.
 */
void SaveArray::addFloatField(const std::string& aName, float aValue)
{
	// Check if the field already exists
	auto it = std::find_if(mFloatFields.begin(), mFloatFields.end(),
						   [&aName](const FloatSaveField& field) { return field.getName() == aName; });
	if (it != mFloatFields.end())
	{
		std::cout << "SaveArray::addFloatField(): cannot add field with name \"" << it->getName()
				  << "\", field already exists. Overwriting value instead" << std::endl;
		it->setValue(aValue);
		return;
	}

	mFloatFields.emplace_back(aName, aValue);
}

/**
 * @brief Adds a string field to the SaveArray. If a field with the same name already exists, the value is overwritten.
 *
 * @param aName The name of the field.
 * @param aValue The value of the field.
 */
void SaveArray::addStringField(const std::string& aName, const std::string& aValue)
{
	// Check if the field already exists
	auto it = std::find_if(mStringFields.begin(), mStringFields.end(),
						   [&aName](const StringSaveField& field) { return field.getName() == aName; });
	if (it != mStringFields.end())
	{
		std::cout << "SaveArray::addStringField(): cannot add field with name \"" << it->getName()
				  << "\", field already exists. Overwriting value instead" << std::endl;
		it->setValue(aValue);
		return;
	}

	mStringFields.emplace_back(aName, aValue);
}

/**
 * @brief Gets an integer field from the SaveArray.
 *
 * @param aName The name of the field.
 *
 * @return The integer field. (as a reference)
 *
 * @throw std::invalid_argument if the field does not exist.
 */
IntSaveField& SaveArray::getIntField(const std::string& aName)
{
	auto it = std::find_if(mIntFields.begin(), mIntFields.end(),
						   [&aName](const IntSaveField& field) { return field.getName() == aName; });
	if (it != mIntFields.end())
	{
		return *it;
	}
	throw std::invalid_argument("Failed to get field " + aName);
}

/**
 * @brief Gets a float field from the SaveArray.
 *
 * @param aName The name of the field.
 *
 * @return The float field. (as a reference)
 *
 * @throw std::invalid_argument if the field does not exist.
 */
FloatSaveField& SaveArray::getFloatField(const std::string& aName)
{
	auto it = std::find_if(mFloatFields.begin(), mFloatFields.end(),
						   [&aName](const FloatSaveField& field) { return field.getName() == aName; });
	if (it != mFloatFields.end())
	{
		return *it;
	}
	throw std::invalid_argument("Failed to get field " + aName);
}

/**
 * @brief Gets a string field from the SaveArray.
 *
 * @param aName The name of the field.
 *
 * @return The string field. (as a reference)
 *
 * @throw std::invalid_argument if the field does not exist.
 */
StringSaveField& SaveArray::getStringField(const std::string& aName)
{
	auto it = std::find_if(mStringFields.begin(), mStringFields.end(),
						   [&aName](const StringSaveField& field) { return field.getName() == aName; });
	if (it != mStringFields.end())
	{
		return *it;
	}
	throw std::invalid_argument("Failed to get field " + aName);
}

/**
 * @brief Gets the integer fields from the SaveArray.
 *
 * @return The integer fields.
 */
const std::vector<IntSaveField>& SaveArray::getIntArray() const { return mIntFields; }

/**
 * @brief Gets the float fields from the SaveArray.
 *
 * @return The float fields.
 */
const std::vector<FloatSaveField>& SaveArray::getFloatArray() const { return mFloatFields; }

/**
 * @brief Gets the string fields from the SaveArray.
 *
 * @return The string fields.
 */
const std::vector<StringSaveField>& SaveArray::getStringArray() const { return mStringFields; }
