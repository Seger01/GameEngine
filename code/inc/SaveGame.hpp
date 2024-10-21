#pragma once

#include "FloatSaveField.hpp"
#include "IntSaveField.hpp"
#include "SaveArray.hpp"
#include "StringSaveField.hpp"
#include <string>

class SaveGame {
public:
  SaveGame(const std::string &aFileName);
  void store();
  void remove();

public:
  void addAnyFromString(std::string aName, std::string aValue);
  void addIntField(std::string name, int value);
  void addFloatField(std::string name, float value);
  void addStringField(std::string name, std::string value);

public:
  IntSaveField &getIntField(std::string aName) const;
  FloatSaveField &getFloatField(std::string aName) const;
  StringSaveField &getStringField(std::string aName) const;

public:
  void addArray(std::string name);
  void setArray(std::string aName, SaveArray aValue);
  SaveArray getArray(std::string aName) const;

private:
  void createFile();

private:
  std::string mFileName;
  std::vector<IntSaveField> mIntFields;
  std::vector<FloatSaveField> mFloatFields;
  std::vector<StringSaveField> mStringFields;
  std::vector<SaveArray> mArrays;
};