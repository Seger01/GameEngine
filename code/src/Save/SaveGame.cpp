#include "SaveGame.h"
#include "FSConverter.h"
#include "SaveGameUtil.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> // Include nlohmann/json

using json = nlohmann::json;

SaveGame::SaveGame(const std::string& aFileName) {
    FSConverter fsConverter;
    mFileName = fsConverter.getResourcePath(aFileName);
    std::ifstream inFile(mFileName);
    if (inFile) {
        // File exists, load the data
        json j;
        inFile >> j;

        // Load fields from JSON
        for (const auto& field : j["fields"]) {
            if (field.contains("name") && field.contains("value")) {
                addAny(field["name"].get<std::string>(), field["value"]);
            } else {
                throw std::logic_error("Failed to load field from JSON: field does not contain \"name\" and \"value\"");
            }
        }

        // Load arrays from JSON
        for (const auto& array : j["arrays"]) {
            if (array.contains("name")) {
                SaveArray saveArray(array["name"]);

                // Add all fields
                for (const auto& arrayField : array["fields"]) {
                    if (arrayField.contains("name") && arrayField.contains("value")) {
                        saveArray.addAny(arrayField["name"].get<std::string>(), arrayField["value"]);
                    }
                }

                mArrays.push_back(saveArray);
            } else {
                throw std::logic_error(
                    "Failed to load array from JSON: array does not contain \"name\" and \"fields\"");
            }
        }
    }
}

void SaveGame::store() {
    json j;

    // Serialize fields (int, then float, then string)
    for (auto& field : mIntFields) {
        j["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
    }
    for (auto& field : mFloatFields) {
        j["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
    }
    for (auto& field : mStringFields) {
        j["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
    }

    // Serialize arrays
    for (auto& array : mArrays) {
        json arrayJson;
        arrayJson["name"] = array.getName();

        for (auto& field : array.getIntArray()) {
            arrayJson["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
        }
        for (auto& field : array.getFloatArray()) {
            arrayJson["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
        }
        for (auto& field : array.getStringArray()) {
            arrayJson["fields"].push_back({{"name", field.getName()}, {"value", field.getValue()}});
        }

        j["arrays"].push_back(arrayJson);
    }

    // Write to file
    std::ofstream outFile(mFileName);
    if (outFile) {
        outFile << j.dump(4); // Pretty print with 4 spaces
    } else {
        throw std::invalid_argument("Failed to open the file: " + mFileName);
    }
}

void SaveGame::addAny(const std::string& aName, const nlohmann::json& aValue) {
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

/**
 * @brief Add an integer field to the save game
 *
 * @param aName The name of the field
 * @param aValue The value of the field
 *
 * @note If a field with the same name already exists, the value will be overwritten
 */
void SaveGame::addIntField(std::string aName, int aValue) {
    // Check if the field already exists
    if (hasIntField(aName)) {
        std::cout << "Field with name \"" << aName << "\" already exists. Overwriting value instead." << std::endl;
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
void SaveGame::addFloatField(std::string aName, float aValue) {
    // Check if the field already exists
    if (hasFloatField(aName)) {
        std::cout << "Field with name \"" << aName << "\" already exists. Overwriting value instead." << std::endl;
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
void SaveGame::addStringField(std::string aName, std::string aValue) {
    // Check if the field already exists
    if (hasStringField(aName)) {
        std::cout << "Field with name \"" << aName << "\" already exists. Overwriting value instead." << std::endl;
        setStringField(aName, aValue);
        return;
    }

    mStringFields.emplace_back(aName, aValue);
}

void SaveGame::setIntField(std::string aName, int aValue) {
    auto it = std::find_if(mIntFields.begin(), mIntFields.end(),
                           [aName](const IntSaveField& field) { return field.getName() == aName; });
    if (it == mIntFields.end()) {
        throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
    }
    it->setValue(aValue);
}

void SaveGame::setFloatField(std::string aName, float aValue) {
    auto it = std::find_if(mFloatFields.begin(), mFloatFields.end(),
                           [aName](const FloatSaveField& field) { return field.getName() == aName; });
    if (it == mFloatFields.end()) {
        throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
    }
    it->setValue(aValue);
}

void SaveGame::setStringField(std::string aName, std::string aValue) {
    auto it = std::find_if(mStringFields.begin(), mStringFields.end(),
                           [aName](const StringSaveField& field) { return field.getName() == aName; });
    if (it == mStringFields.end()) {
        throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
    }
    it->setValue(aValue);
}

bool SaveGame::hasIntField(std::string aName) const {
    // any_of checks if any element in the range satisfies the condition
    return std::any_of(mIntFields.begin(), mIntFields.end(),
                       [aName](const IntSaveField& field) { return field.getName() == aName; });
}

bool SaveGame::hasFloatField(std::string aName) const {
    // any_of checks if any element in the range satisfies the condition
    return std::any_of(mFloatFields.begin(), mFloatFields.end(),
                       [aName](const FloatSaveField& field) { return field.getName() == aName; });
}

bool SaveGame::hasStringField(std::string aName) const {
    // any_of checks if any element in the range satisfies the condition
    return std::any_of(mStringFields.begin(), mStringFields.end(),
                       [aName](const StringSaveField& field) { return field.getName() == aName; });
}

void SaveGame::remove() {
    if (std::remove(mFileName.c_str()) != 0) {
        throw std::logic_error("Failed to remove the file: " + mFileName);
    }
}

const IntSaveField& SaveGame::getIntField(std::string aName) const {
    auto it = std::find_if(mIntFields.begin(), mIntFields.end(),
                           [aName](const IntSaveField& field) { return field.getName() == aName; });
    if (it != mIntFields.end()) {
        return *it;
    }
    throw std::invalid_argument("Failed to get field with name \"" + aName + "\"");
}

const FloatSaveField& SaveGame::getFloatField(std::string aName) const {
    auto it = std::find_if(mFloatFields.begin(), mFloatFields.end(),
                           [aName](const FloatSaveField& field) { return field.getName() == aName; });
    if (it != mFloatFields.end()) {
        return *it;
    }
    throw std::invalid_argument("Failed to get field with name \"" + aName + "\"");
}

const StringSaveField& SaveGame::getStringField(std::string aName) const {
    auto it = std::find_if(mStringFields.begin(), mStringFields.end(),
                           [aName](const StringSaveField& field) { return field.getName() == aName; });
    if (it != mStringFields.end()) {
        return *it;
    }
    throw std::invalid_argument("Failed to get field with name \"" + aName + "\"");
}

void SaveGame::addArray(std::string aName) {
    // Check if the array already exists
    auto it = std::find_if(mArrays.begin(), mArrays.end(),
                           [aName](const SaveArray& array) { return array.getName() == aName; });
    if (it != mArrays.end()) {
        // Array already exists
        throw std::invalid_argument("Failed to add array with name \"" + aName + "\". Array already exists.");
    }

    mArrays.emplace_back(aName);
}

void SaveGame::setArray(std::string aName, SaveArray aValue) {
    // Search for the array
    auto it = std::find_if(mArrays.begin(), mArrays.end(),
                           [aName](const SaveArray& array) { return array.getName() == aName; });
    if (it == mArrays.end()) {
        // Array does not exist
        throw std::invalid_argument("Failed to set array with name \"" + aName + "\". Array does not exist.");
    }

    // Overwrite the array
    *it = aValue;
}

SaveArray SaveGame::getArray(std::string aName) const {
    auto it = std::find_if(mArrays.begin(), mArrays.end(),
                           [aName](const SaveArray& array) { return array.getName() == aName; });
    if (it == mArrays.end()) {
        throw std::invalid_argument("Failed to get array with name \"" + aName + "\"");
    }
    return *it;
}