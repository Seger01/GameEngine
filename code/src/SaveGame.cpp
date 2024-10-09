#include "SaveGame.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> // Include nlohmann/json

using json = nlohmann::json;

SaveGame::SaveGame(const std::string &aFileName) : mFileName(aFileName) {
  std::ifstream inFile(mFileName);
  if (inFile) {
    // File exists, load the data
    json j;
    inFile >> j;

    // Load fields from JSON
    if (j.contains("fields")) {
      for (const auto &field : j["fields"]) {
        if (field.contains("name") && field.contains("value")) {
          SaveField saveField(field["name"]);
          saveField.setValue(field["value"]);
          mFields.push_back(saveField);
        }
      }
    }

    // Load arrays from JSON
    if (j.contains("arrays")) {
      for (const auto &array : j["arrays"]) {
        if (array.contains("name") && array.contains("fields")) {
          SaveArray saveArray(array["name"]);

          // Only add fields if they exist
          if (!array["fields"].empty()) {
            for (const auto &arrayField : array["fields"]) {
              if (arrayField.contains("name") && arrayField.contains("value")) {
                saveArray.addField(arrayField["name"], arrayField["value"]);
              }
            }
          }

          mArrays.push_back(saveArray);
        }
      }
    }
  } else {
    // File doesn't exist, create a new one
    createFile();
  }
}

void SaveGame::createFile() {
  std::ofstream outFile(mFileName);
  if (!outFile) {
    std::cerr << "Failed to create the file: " << mFileName << std::endl;
  }
}

void SaveGame::store() {
  json j;

  // Serialize fields
  for (auto &field : mFields) {
    j["fields"].push_back(
        {{"name", field.getName()}, {"value", field.getValue()}});
  }

  // Serialize arrays
  for (auto &array : mArrays) {
    json arrayJson;
    arrayJson["name"] = array.getName();

    for (auto &field :
         array
             .getArray()) { // assuming public access to mFields or add a getter
      arrayJson["fields"].push_back(
          {{"name", field.getName()}, {"value", field.getValue()}});
    }
    j["arrays"].push_back(arrayJson);
  }

  // Write to file
  std::ofstream outFile(mFileName);
  if (outFile) {
    outFile << j.dump(4); // Pretty print with 4 spaces
  } else {
    std::cerr << "Failed to open the file: " << mFileName << std::endl;
  }
}

void SaveGame::addField(std::string aName, std::string aValue) {
  // Checkif the field already exists
  try {
    for (auto &field : mFields) {
      if (field.getName() == aName) {
        throw(aName);
      }
    }
  } catch (std::string val) {
    std::cout << "SaveGame::addField(): cannot add field with name \"" << aName
              << "\", field already exists." << std::endl;
    return;
  }

  SaveField newField{aName};
  newField.setValue(aValue);
  mFields.push_back(newField);
}

void SaveGame::remove() { std::remove(mFileName.c_str()); }

void SaveGame::setField(std::string aName, std::string aValue) {
  try {
    for (SaveField &field : mFields) {
      if (field.getName() == aName) {
        field.setValue(aValue);
        return;
      }
    }
    throw(aName);
  } catch (std::string value) {
    std::cout << "SaveGame::setField(): failed to find field with name \""
              << aName << "\"" << std::endl;
  }
}
std::string SaveGame::getField(std::string aName) const {
  try {
    for (SaveField field : mFields) {
      if (field.getName() == aName) {
        return field.getValue();
      }
    }
    throw(aName);
  } catch (std::string aName) {
    std::cout << "SaveGame::getField(): failed to find field with name \""
              << aName << "\"" << std::endl;
    return "";
  }
}

void SaveGame::addArray(std::string aName) {
  // Check if the array already exists
  try {
    for (auto &array : mArrays) {
      if (array.getName() == aName) {
        throw(aName);
      }
    }
  } catch (std::string val) {
    std::cout << "SaveGame::addArray(): cannot add array with name \"" << aName
              << "\", field already exists." << std::endl;
    return;
  }

  SaveArray array(aName);
  mArrays.push_back(array);
}

void SaveGame::setArray(std::string aName, SaveArray aValue) {
  try {
    for (SaveArray &array : mArrays) {
      if (array.getName() == aName) {
        array = aValue;
        return;
      }
    }
    throw(aName);
  } catch (std::string aName) {
    std::cout << "SaveArray::setArray(): failed to find array with name \""
              << aName << "\"" << std::endl;
    return;
  }
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
    std::cout << "SaveArray::getField(): failed to find field with name \""
              << aName << "\"" << std::endl;
    return mArrays[0];
  }
}