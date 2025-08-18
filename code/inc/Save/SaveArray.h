/**
 * @file SaveArray.h
 *
 * Contains the declaration of the SaveArray class.
 */
#pragma once

#include "Save/FloatSaveField.h"
#include "Save/IntSaveField.h"
#include "Save/StringSaveField.h"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

/**
 * @class SaveArray
 *
 * @brief A class representing an array of fields in a save game. Contains a name and three vectors (one of each save
 * field type).
 */
class SaveArray
{
public:
	SaveArray(const std::string& aName);

public:
	std::string getName() const;

public:
	void addIntField(const std::string& aName, int aValue);
	void addFloatField(const std::string& aName, float aValue);
	void addStringField(const std::string& aName, const std::string& aValue);

public:
	IntSaveField& getIntField(const std::string& aName);
	FloatSaveField& getFloatField(const std::string& aName);
	StringSaveField& getStringField(const std::string& aName);

public:
	const std::vector<IntSaveField>& getIntArray() const;
	const std::vector<FloatSaveField>& getFloatArray() const;
	const std::vector<StringSaveField>& getStringArray() const;

protected:
	void addAny(const std::string& aName, const nlohmann::json& aValue);

private:
	/// @brief The name of the array. Used to identify it.
	std::string mName;
	/// @brief Vector of integer fields stored in this array.
	std::vector<IntSaveField> mIntFields;
	/// @brief Vector of float fields stored in this array.
	std::vector<FloatSaveField> mFloatFields;
	/// @brief Vector of string fields stored in this array.
	std::vector<StringSaveField> mStringFields;
	/// @brief Allow SaveGame to access private members.
	friend class SaveGame;
};
