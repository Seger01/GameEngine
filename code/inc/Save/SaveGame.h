#pragma once

#include "SaveArray.h"
#include "IntSaveField.h"
#include "FloatSaveField.h"
#include "StringSaveField.h"
//#include "SaveField.hpp"
#include <string>
#include <vector>

class SaveGame {
public:
  SaveGame(const std::string &aFileName);

public:
  void store();
  void remove();
  void addIntField(std::string name, int value);
  void addFloatField(std::string name, float value);
  void addStringField(std::string name, std::string value);
  IntSaveField& getIntField(std::string name);
  FloatSaveField& getFloatField(std::string name);
  StringSaveField& getStringField(std::string name);
  
  void addArray(std::string name);
  void setArray(std::string name, SaveArray value);
  SaveArray getArray(std::string name) const;

private:
  void createFile();
  std::string mFileName;
  //std::vector<SaveField> mFields;
  std::vector<SaveArray> mArrays;
};