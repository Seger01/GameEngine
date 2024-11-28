#include "SaveArray.h"
#include "FloatSaveField.h"
#include "IntSaveField.h"
#include "SaveGame.h"
#include "SaveGameUtil.h"
#include "StringSaveField.h"
#include <algorithm> // Include the algorithm header for std::find_if

#include <iostream>

SaveArray::SaveArray(const std::string& aName) : mName(aName) {}

std::string SaveArray::getName() const { return mName; }

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

const std::vector<IntSaveField>& SaveArray::getIntArray() const { return mIntFields; }

const std::vector<FloatSaveField>& SaveArray::getFloatArray() const { return mFloatFields; }

const std::vector<StringSaveField>& SaveArray::getStringArray() const { return mStringFields; }