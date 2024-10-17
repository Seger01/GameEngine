#pragma once

#include <string>

class IntSaveField {
public:
  IntSaveField(std::string name) : mName(name) {}

  std::string getName() const { return mName; }

  void setValue(int aValue) { mValue = aValue; }

  int getValue() const { return mValue; }

private:
  std::string mName;
  int mValue;
};