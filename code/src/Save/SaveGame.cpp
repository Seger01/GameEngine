#include "SaveGame.h"
#include "SaveGameUtil.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> // Include nlohmann/json

using json = nlohmann::json;

SaveGame::SaveGame(const std::string& aFileName) : mFileName(aFileName) {
    std::ifstream inFile(mFileName);
    if (inFile) {
        // File exists, load the data
        json j;
        inFile >> j;

        // Load fields from JSON
        if (j.contains("fields")) {
            for (const auto& field : j["fields"]) {
                if (field.contains("name") && field.contains("value")) {
                    addAny(field["name"].get<std::string>(), field["value"]);
                }
            }
        }

        // Load arrays from JSON
        if (j.contains("arrays")) {
            for (const auto& array : j["arrays"]) {
                if (array.contains("name") && array.contains("fields")) {
                    SaveArray saveArray(array["name"]);

                    // Only add fields if they exist
                    if (!array["fields"].empty()) {
                        for (const auto& arrayField : array["fields"]) {
                            if (arrayField.contains("name") && arrayField.contains("value")) {
                                saveArray.addAny(arrayField["name"].get<std::string>(), arrayField["value"]);
                            }
                        }
                    }

                    mArrays.push_back(saveArray);
                }
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
    try {
        for (auto& field : mIntFields) {
            if (field.getName() == aName) {
                std::cout << "Field with name \"" << field.getName() << "\" already exists. Overwriting value instead."
                          << std::endl;
                field.setValue(aValue);
                return;
            }
        }
    } catch (std::string val) {
        std::cout << "SaveGame::addIntField(): cannot add field with name \"" << aName << "\", field already exists."
                  << std::endl;
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
    try {
        for (auto& field : mFloatFields) {
            if (field.getName() == aName) {
                std::cout << "Field with name \"" << field.getName() << "\" already exists. Overwriting value instead."
                          << std::endl;
                field.setValue(aValue);
                return;
            }
        }
    } catch (std::string val) {
        std::cout << "SaveGame::addFloatField(): cannot add field with name \"" << aName << "\", field already exists."
                  << std::endl;
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
    try {
        for (auto& field : mStringFields) {
            if (field.getName() == aName) {
                std::cout << "Field with name \"" << field.getName() << "\" already exists. Overwriting value instead."
                          << std::endl;
                field.setValue(aValue);
                return;
            }
        }
    } catch (std::string val) {
        std::cout << "SaveGame::addStringField(): cannot add field with name \"" << aName << "\", field already exists."
                  << std::endl;
        return;
    }

    mStringFields.emplace_back(aName, aValue);
}

void SaveGame::setIntField(std::string aName, int aValue) {
    for (auto& field : mIntFields) {
        if (field.getName() == aName) {
            field.setValue(aValue);
            return;
        }
    }
    throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
}

void SaveGame::setFloatField(std::string aName, float aValue) {
    for (auto& field : mFloatFields) {
        if (field.getName() == aName) {
            field.setValue(aValue);
            return;
        }
    }
    throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
}

void SaveGame::setStringField(std::string aName, std::string aValue) {
    for (auto& field : mStringFields) {
        if (field.getName() == aName) {
            field.setValue(aValue);
            return;
        }
    }
    throw std::invalid_argument("Failed to find field with name \"" + aName + "\"");
}

bool SaveGame::hasIntField(std::string aName) const {
    for (const auto& field : mIntFields) {
        if (field.getName() == aName) {
            return true;
        }
    }
    return false;
}

bool SaveGame::hasFloatField(std::string aName) const {
    for (const auto& field : mFloatFields) {
        if (field.getName() == aName) {
            return true;
        }
    }
    return false;
}

bool SaveGame::hasStringField(std::string aName) const {
    for (const auto& field : mStringFields) {
        if (field.getName() == aName) {
            return true;
        }
    }
    return false;
}

void SaveGame::remove() { std::remove(mFileName.c_str()); }

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
    try {
        for (auto& array : mArrays) {
            if (array.getName() == aName) {
                throw(aName);
            }
        }
    } catch (std::string val) {
        std::cout << "SaveGame::addArray(): cannot add array with name \"" << aName << "\", field already exists."
                  << std::endl;
        return;
    }

    SaveArray array(aName);
    mArrays.push_back(array);
}

void SaveGame::setArray(std::string aName, SaveArray aValue) {

    for (SaveArray& array : mArrays) {
        if (array.getName() == aName) {
            array = aValue;
            return;
        }
    }
    throw std::invalid_argument("Failed to find array with name \"" + aName + "\"");
}

SaveArray SaveGame::getArray(std::string aName) const {
    try {
        for (SaveArray array : mArrays) {
            if (array.getName() == aName) {
                return array;
            }
        }
        throw(aName);
    } catch (std::string aName) {
        std::cout << "SaveArray::getField(): failed to find field with name \"" << aName << "\"" << std::endl;
        return mArrays[0];
    }
}

void SaveGame::createFile() {
    std::ofstream outFile(mFileName);
    if (!outFile) {
        std::cerr << "Failed to create the file: " << mFileName << std::endl;
    }
}