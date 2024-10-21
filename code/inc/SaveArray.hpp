#pragma once

#include "FloatSaveField.hpp"
#include "IntSaveField.hpp"
#include "StringSaveField.hpp"
#include <string>
#include <vector>

class SaveArray {
public:
  SaveArray(std::string name);

public:
  std::string getName() const;

public:
  void addIntField(std::string name, int value);
  void addFloatField(std::string name, float value);
  void addStringField(std::string name, std::string value);

public:
  IntSaveField &getIntField(std::string aName) const;
  FloatSaveField &getFloatField(std::string aName) const;
  StringSaveField &getStringField(std::string aName) const;

public:
  const std::vector<IntSaveField> &getIntArray() const;
  const std::vector<FloatSaveField> &getFloatArray() const;
  const std::vector<StringSaveField> &getStringArray() const;

protected:
  void addAnyFromString(std::string aName, std::string aValue);

private:
  std::string mName;
  std::vector<IntSaveField> mIntFields;
  std::vector<FloatSaveField> mFloatFields;
  std::vector<StringSaveField> mStringFields;

  friend class SaveGame;
};