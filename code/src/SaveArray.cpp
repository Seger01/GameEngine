#include "SaveArray.hpp"

#include "SaveField.hpp"
#include <iostream>

SaveArray::SaveArray(std::string name) : mName(name) {}

void SaveArray::addField(std::string name, std::string value) {
  SaveField newField{name};
  newField.setValue(value);
  mFields.push_back(newField);
}

std::string SaveArray::getName() const { return mName; }

std::vector<SaveField> SaveArray::getArray() const { return mFields; }

void SaveArray::setField(std::string aName, std::string aValue) {
  try {
    for (SaveField field : mFields) {
      if (field.getName() == aName) {
        field.setValue(aValue);
        return;
      }
    }
    throw(aName);
  } catch (std::string value) {
    std::cout << "SaveArray::setField(): failed to find field with name \""
              << aName << "\"" << std::endl;
  }
}
std::string SaveArray::getField(std::string aName) const {
  try {
    for (SaveField field : mFields) {
      if (field.getName() == aName) {
        return field.getValue();
      }
    }
    throw(aName);
  } catch (std::string aName) {
    std::cout << "SaveArray::getField(): failed to find field with name \""
              << aName << "\"" << std::endl;
    return "";
  }
}