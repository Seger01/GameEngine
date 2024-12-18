#include "SaveGame.h"
#include "FSConverter.h"
#include "SaveGameUtil.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @brief Constructor to initialize the SaveGame with a file name. If the file exists, the data is loaded.
 *
 * @param aFileName The name of the file to load/save the data.
 *
 * @throw std::logic_error if a field or array does not contain "name" and "value" ("fields" in the case of an array).
 */
SaveGame::SaveGame(const std::string& aFileName)
{
	FSConverter fsConverter;
	mFileName = fsConverter.getResourcePath(aFileName);
	std::ifstream inFile(mFileName);
	if (inFile)
	{
		// File exists, load the data
		json j;
		inFile >> j;

		// Load fields from JSON
		for (const auto& field : j["fields"])
		{
			if (field.contains("name") && field.contains("value"))
			{
				addAny(field["name"].get<std::string>(), field["value"]);
			}
			else
			{
				throw std::logic_error("Failed to load field from JSON: field does not contain \"name\" and \"value\"");
			}
		}

		// Load arrays from JSON
		for (const auto& array : j["arrays"])
		{
			if (array.contains("name"))
			{
				SaveArray saveArray(array["name"]);

				// Add all fields
				for (const auto& arrayField : array["fields"])
				{
					if (arrayField.contains("name") && arrayField.contains("value"))
					{
						saveArray.addAny(arrayField["name"].get<std::string>(), arrayField["value"]);
					}
				}

				mArrays.push_back(saveArray);
			}
			else
			{
				throw std::logic_error(
					"Failed to load array from JSON: array does not contain \"name\" and \"fields\"");
			}
		}
	}
}

/**
 * @brief Store the data to the specified mFileName.
 *
 * @throw std::invalid_argument if the file cannot be opened.
 */
void SaveGame::store()
{
	json j;

	// Serialize fields (int, then float, then string)
	for (auto& field : mIntFields)
	{
		j["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
	}
	for (auto& field : mFloatFields)
	{
		j["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
	}
	for (auto& field : mStringFields)
	{
		j["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
	}

	// Serialize arrays
	for (auto& array : mArrays)
	{
		json arrayJson;
		arrayJson["name"] = array.getName();

		for (auto& field : array.getIntArray())
		{
			arrayJson["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
		}
		for (auto& field : array.getFloatArray())
		{
			arrayJson["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
		}
		for (auto& field : array.getStringArray())
		{
			arrayJson["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
		}

		j["arrays"].push_back(arrayJson);
	}

	// Write to file
	std::ofstream outFile(mFileName);
	if (outFile)
	{
		outFile << j.dump(4); // Pretty print with 4 spaces
	}
	else
	{
		throw std::invalid_argument("Failed to open the file: " + mFileName);
	}
}

/**
 * @brief Add a field to the save game. The type of the field is determined by the value.
 *
 * @param aName The name of the field
 * @param aValue The value of the field
 *
 * @throw std::invalid_argument if the value is not an integer, float, or string
 */
void SaveGame::addAny(const std::string& aName, const nlohmann::json& aValue)
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
 * @brief Add an integer field to the save game
 *
 * @param aName The name of the field
 * @param aValue The value of the field
 *
 * @note If a field with the same name already exists, the value will be overwritten
 */
void SaveGame::addIntField(const std::string& aName, int aValue)
{
	// Check if the field already exists
	if (hasIntField(aName))
	{
		setIntField(aName, aValue);
		return;
	}

	mIntFields.emplace_back(aName, aValue);
}

/**
 * @brief Add a float field to the save game
 *
 * @param aName The name of the field
 * @param aValue The value of the field
 *
 * @note If a field with the same name already exists, the value will be overwritten
 */
void SaveGame::addFloatField(const std::string& aName, float aValue)
{
	// Check if the field already exists
	if (hasFloatField(aName))
	{
		setFloatField(aName, aValue);
		return;
	}

	mFloatFields.emplace_back(aName, aValue);
}

/**
 * @brief Add a string field to the save game
 *
 * @param aName The name of the field
 * @param aValue The value of the field
 *
 * @note If a field with the same name already exists, the value will be overwritten
 */
void SaveGame::addStringField(const std::string& aName, const std::string& aValue)
{
	// Check if the field already exists
	if (hasStringField(aName))
	{
		setStringField(aName, aValue);
		return;
	}

	mStringFields.emplace_back(aName, aValue);
}

/**
 * @brief Set the value of an integer field
 *
 * @param aName The name of the field
 * @param aValue The new value of the field
 *
 * @throw std::invalid_argument if the field does not exist
 */
void SaveGame::setIntField(const std::string& aName, int aValue)
{
	auto it = std::find_if(mIntFields.begin(), mIntFields.end(),
						   [aName](const IntSaveField& field) { return field.getName() == aName; });
	if (it == mIntFields.end())
	{
		throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
	}
	it->setValue(aValue);
}

/**
 * @brief Set the value of a float field
 *
 * @param aName The name of the field
 * @param aValue The new value of the field
 *
 * @throw std::invalid_argument if the field does not exist
 */
void SaveGame::setFloatField(const std::string& aName, float aValue)
{
	auto it = std::find_if(mFloatFields.begin(), mFloatFields.end(),
						   [aName](const FloatSaveField& field) { return field.getName() == aName; });
	if (it == mFloatFields.end())
	{
		throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
	}
	it->setValue(aValue);
}

/**
 * @brief Set the value of a string field
 *
 * @param aName The name of the field
 * @param aValue The new value of the field
 *
 * @throw std::invalid_argument if the field does not exist
 */
void SaveGame::setStringField(const std::string& aName, const std::string& aValue)
{
	auto it = std::find_if(mStringFields.begin(), mStringFields.end(),
						   [aName](const StringSaveField& field) { return field.getName() == aName; });
	if (it == mStringFields.end())
	{
		throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
	}
	it->setValue(aValue);
}

/**
 * @brief Check if an integer field exists
 *
 * @param aName The name of the field
 *
 * @return true if the field exists, false otherwise
 */
bool SaveGame::hasIntField(const std::string& aName) const
{
	// any_of checks if any element in the range satisfies the condition
	return std::any_of(mIntFields.begin(), mIntFields.end(),
					   [aName](const IntSaveField& field) { return field.getName() == aName; });
}

/**
 * @brief Check if a float field exists
 *
 * @param aName The name of the field
 *
 * @return true if the field exists, false otherwise
 */
bool SaveGame::hasFloatField(const std::string& aName) const
{
	// any_of checks if any element in the range satisfies the condition
	return std::any_of(mFloatFields.begin(), mFloatFields.end(),
					   [aName](const FloatSaveField& field) { return field.getName() == aName; });
}

/**
 * @brief Check if a string field exists
 *
 * @param aName The name of the field
 *
 * @return true if the field exists, false otherwise
 */
bool SaveGame::hasStringField(const std::string& aName) const
{
	// any_of checks if any element in the range satisfies the condition
	return std::any_of(mStringFields.begin(), mStringFields.end(),
					   [aName](const StringSaveField& field) { return field.getName() == aName; });
}

/**
 * @brief Remove the file associated with the save game
 *
 * @throw std::logic_error if the file cannot be removed
 */
void SaveGame::remove()
{
	if (std::remove(mFileName.c_str()) != 0)
	{
		throw std::logic_error("Failed to remove the file: " + mFileName);
	}
}

/**
 * @brief Get an integer field from the save game
 *
 * @param aName The name of the field
 *
 * @return The integer field
 *
 * @throw std::invalid_argument if the field does not exist
 */
const IntSaveField& SaveGame::getIntField(const std::string& aName) const
{
	auto it = std::find_if(mIntFields.begin(), mIntFields.end(),
						   [aName](const IntSaveField& field) { return field.getName() == aName; });
	if (it != mIntFields.end())
	{
		return *it;
	}
	throw std::invalid_argument("Failed to get field with name \"" + aName + "\"");
}

/**
 * @brief Get a float field from the save game
 *
 * @param aName The name of the field
 *
 * @return The float field
 *
 * @throw std::invalid_argument if the field does not exist
 */
const FloatSaveField& SaveGame::getFloatField(const std::string& aName) const
{
	auto it = std::find_if(mFloatFields.begin(), mFloatFields.end(),
						   [aName](const FloatSaveField& field) { return field.getName() == aName; });
	if (it != mFloatFields.end())
	{
		return *it;
	}
	throw std::invalid_argument("Failed to get field with name \"" + aName + "\"");
}

/**
 * @brief Get a string field from the save game
 *
 * @param aName The name of the field
 *
 * @return The string field
 *
 * @throw std::invalid_argument if the field does not exist
 */
const StringSaveField& SaveGame::getStringField(const std::string& aName) const
{
	auto it = std::find_if(mStringFields.begin(), mStringFields.end(),
						   [aName](const StringSaveField& field) { return field.getName() == aName; });
	if (it != mStringFields.end())
	{
		return *it;
	}
	throw std::invalid_argument("Failed to get field with name \"" + aName + "\"");
}

/**
 * @brief Add an array to the save game
 *
 * @param aName The name of the array
 *
 * @throw std::invalid_argument if the array already exists
 */
void SaveGame::addArray(const std::string& aName)
{
	// Check if the array already exists
	auto it = std::find_if(mArrays.begin(), mArrays.end(),
						   [aName](const SaveArray& array) { return array.getName() == aName; });
	if (it != mArrays.end())
	{
		// Array already exists
		throw std::invalid_argument("Failed to add array with name \"" + aName + "\". Array already exists.");
	}

	mArrays.emplace_back(aName);
}

/**
 * @brief Set the value of an array
 *
 * @param aName The name of the array
 * @param aValue The new value of the array
 *
 * @throw std::invalid_argument if the array does not exist
 */
void SaveGame::setArray(const std::string& aName, const SaveArray& aValue)
{
	// Search for the array
	auto it = std::find_if(mArrays.begin(), mArrays.end(),
						   [aName](const SaveArray& array) { return array.getName() == aName; });
	if (it == mArrays.end())
	{
		// Array does not exist
		throw std::invalid_argument("Failed to set array with name \"" + aName + "\". Array does not exist.");
	}

	// Overwrite the array
	*it = aValue;
}

/**
 * @brief Get an array from the save game
 *
 * @param aName The name of the array
 *
 * @return The array
 *
 * @throw std::invalid_argument if the array does not exist
 */
SaveArray SaveGame::getArray(const std::string& aName) const
{
	auto it = std::find_if(mArrays.begin(), mArrays.end(),
						   [aName](const SaveArray& array) { return array.getName() == aName; });
	if (it == mArrays.end())
	{
		throw std::invalid_argument("Failed to get array with name \"" + aName + "\"");
	}
	return *it;
}
