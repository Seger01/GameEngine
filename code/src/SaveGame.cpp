#include "SaveGame.hpp"
#include <iostream>

SaveGame::SaveGame(std::string aFileName) : mFileName(aFileName) {}

void SaveGame::store() {}

void SaveGame::addField(std::string name, std::string value) {
  SaveField newField{name};
  newField.setValue(value);
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