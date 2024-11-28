#pragma once

#include "FloatSaveField.h"
#include "IntSaveField.h"
#include "StringSaveField.h"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

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
	std::string mName;
	std::vector<IntSaveField> mIntFields;
	std::vector<FloatSaveField> mFloatFields;
	std::vector<StringSaveField> mStringFields;

	friend class SaveGame;
};