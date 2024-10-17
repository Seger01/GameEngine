#pragma once

#include <string>

class FloatSaveField {
public:
  FloatSaveField(std::string name) : mName(name) {}

  std::string getName() const { return mName; }

  void setValue(float aValue) { mValue = aValue; }

  float getValue() const { return mValue; }

private:
  std::string mName;
  float mValue;
};