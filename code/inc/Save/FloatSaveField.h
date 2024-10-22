#pragma once

#include <string>

class FloatSaveField {
public:
  FloatSaveField(std::string name);
  std::string getName() const;
  void setValue(float aValue);
  float getValue() const;

private:
  std::string mName;
  float mValue;
};