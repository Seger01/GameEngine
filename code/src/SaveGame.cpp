#include "SaveGame.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> // Include nlohmann/json

using json = nlohmann::json;

SaveGame::SaveGame(std::string aFileName) : mFileName(aFileName) {
  std::ifstream inFile(mFileName);
  if (inFile) {
    // File exists, load the data
    json j;
    inFile >> j;

    // Load fields from JSON
    for (const auto &field : j["fields"]) {
      SaveField saveField(field["name"]);
      saveField.setValue(field["value"]);
      mFields.push_back(saveField);
    }

    // Load arrays from JSON
    for (const auto &array : j["arrays"]) {
      SaveArray saveArray(array["name"]);
      for (const auto &arrayField : array["fields"]) {
        saveArray.addField(arrayField["name"], arrayField["value"]);
      }
      mArrays.push_back(saveArray);
    }
  } else {
    // File doesn't exist, create a new one
    std::ofstream outFile(mFileName);
    if (!outFile) {
      std::cerr << "Failed to create the file: " << mFileName << std::endl;
    }
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
void SaveGame::setField(std::string aName, std::string aValue) {
  try {
    for (SaveField field : mFields) {
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
std::string SaveGame::getField(std::string aName) {
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
  // todo: check if item exists yet
  SaveArray array(aName);
  mArrays.push_back(array);
}

const SaveArray &SaveGame::getArray(std::string aName) {
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