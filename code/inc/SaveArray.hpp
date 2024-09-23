#pragma once

#include "SaveField.hpp"
#include <string>
#include <vector>

class SaveArray {
public:
  SaveArray(std::string name);

public:
  std::string getName();

public:
  void addField(std::string name, std::string value);
  void setField(std::string name, std::string value);
  std::string getField(std::string aName);

private:
  std::string mName;
  std::vector<SaveField> mFields;
};