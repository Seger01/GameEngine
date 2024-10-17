#pragma once

#include <string>

class StringSaveField {
public:
  StringSaveField(std::string name) : mName(name) {}

  std::string getName() const { return mName; }

  void setValue(std::string aValue) { mValue = aValue; }

  std::string getValue() const { return mValue; }

private:
  std::string mName;
  std::string mValue;
};