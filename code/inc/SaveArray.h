#pragma once

//#include "SaveField.hpp"
#include <string>
#include <vector>
#include "IntSaveField.h"
#include "FloatSaveField.h"
#include "StringSaveField.h"

class SaveArray {
public:
  SaveArray(std::string name);
  std::string getName() const;
  //std::vector<SaveField<std::string>> getArray() const;
  void addIntField(std::string name, int value);
  void addFloatField(std::string name, float value);
  void addStringField(std::string name, std::string value);
  IntSaveField& getIntField(std::string name);
  FloatSaveField& getFloatField(std::string name);
  StringSaveField& getStringField(std::string name);

private:
  std::string mName;
  std::vector<IntSaveField> mIntFields;
  std::vector<FloatSaveField> mFloatFields;
  std::vector<StringSaveField> mStringFields;
};