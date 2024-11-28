/**
 * @file SaveGame.h
 *
 * Contains the declaration of the SaveGame class.
 */
#pragma once

#include "FloatSaveField.h"
#include "IntSaveField.h"
#include "SaveArray.h"
#include "StringSaveField.h"

#include <string>

/**
 * @class SaveGame
 *
 * @brief A class representing a save game. Contains a file name and vectors of each save field type. Also contains a
 * vector of SaveArray objects.
 */
class SaveGame
{
public:
	SaveGame(const std::string& aFileName);
	void store();
	void remove();

public:
	void addIntField(const std::string& aName, int value);
	void addFloatField(const std::string& aName, float value);
	void addStringField(const std::string& aName, const std::string& value);

public:
	void setIntField(const std::string& aName, int aValue);
	void setFloatField(const std::string& aName, float aValue);
	void setStringField(const std::string& aName, const std::string& aValue);

public:
	bool hasIntField(const std::string& aName) const;
	bool hasFloatField(const std::string& aName) const;
	bool hasStringField(const std::string& aName) const;

public:
	const IntSaveField& getIntField(const std::string& aName) const;
	const FloatSaveField& getFloatField(const std::string& aName) const;
	const StringSaveField& getStringField(const std::string& aName) const;

public:
	void addArray(const std::string& aName);
	void setArray(const std::string& aName, const SaveArray& aValue);
	SaveArray getArray(const std::string& aName) const;

private:
	void addAny(const std::string& aName, const nlohmann::json& aValue);

private:
	std::string mFileName;
	std::vector<IntSaveField> mIntFields;
	std::vector<FloatSaveField> mFloatFields;
	std::vector<StringSaveField> mStringFields;
	std::vector<SaveArray> mArrays;
};