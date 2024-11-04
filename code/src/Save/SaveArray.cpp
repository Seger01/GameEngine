#include "SaveArray.h"
#include "FloatSaveField.h"
#include "IntSaveField.h"
#include "SaveGame.h"
#include "SaveGameUtil.h"
#include "StringSaveField.h"
#include <iostream>

SaveArray::SaveArray(const std::string aName) : mName(aName) {}

std::string SaveArray::getName() const { return mName; }

void SaveArray::addAny(const std::string& aName, const nlohmann::json& aValue) {
    if (aValue.is_number_integer()) {
        int int_value = aValue.get<int>();
        addIntField(aName, int_value);
    } else if (aValue.is_number_float()) {
        float float_value = aValue.get<float>();
        addFloatField(aName, float_value);
    } else if (aValue.is_string()) {
        std::string str_value = aValue.get<std::string>();
        addStringField(aName, str_value);
    } else {
        throw std::invalid_argument("Failed to add field with name \"" + aName + "\". Invalid type.");
    }
}

void SaveArray::addIntField(std::string aName, int aValue) {
    // Check if the field already exists
    for (auto& field : mIntFields) {
        if (field.getName() == aName) {
            std::cout << "SaveArray::addIntField(): cannot add field with name \"" << field.getName()
                      << "\", field already exists. Overwriting value instead" << std::endl;
            field.setValue(aValue);
            return;
        }
    }

    mIntFields.emplace_back(aName, aValue);
}

void SaveArray::addFloatField(std::string aName, float aValue) {
    // Check if the field already exists
    for (auto& field : mFloatFields) {
        if (field.getName() == aName) {
            std::cout << "SaveArray::addFloatField(): cannot add field with name \"" << field.getName()
                      << "\", field already exists. Overwriting value instead" << std::endl;
            field.setValue(aValue);
            return;
        }
    }

    mFloatFields.emplace_back(aName, aValue);
}

void SaveArray::addStringField(std::string aName, std::string aValue) {
    // Check if the field already exists
    for (auto& field : mStringFields) {
        if (field.getName() == aName) {
            std::cout << "SaveArray::addStringField(): cannot add field with name \"" << field.getName()
                      << "\", field already exists. Overwriting value instead" << std::endl;
            field.setValue(aValue);
            return;
        }
    }

    mStringFields.emplace_back(aName, aValue);
}

IntSaveField& SaveArray::getIntField(std::string aName) {
    for (IntSaveField& field : mIntFields) {
        if (field.getName() == aName) {
            return field;
        }
    }
    throw("Failed to get field " + aName);
}

FloatSaveField& SaveArray::getFloatField(std::string aName) {
    for (FloatSaveField& field : mFloatFields) {
        if (field.getName() == aName) {
            return field;
        }
    }
    throw("Failed to get field " + aName);
}

StringSaveField& SaveArray::getStringField(std::string aName) {
    for (StringSaveField& field : mStringFields) {
        if (field.getName() == aName) {
            return field;
        }
    }
    throw("Failed to get field " + aName);
}

const std::vector<IntSaveField>& SaveArray::getIntArray() const { return mIntFields; }

const std::vector<FloatSaveField>& SaveArray::getFloatArray() const { return mFloatFields; }

const std::vector<StringSaveField>& SaveArray::getStringArray() const { return mStringFields; }