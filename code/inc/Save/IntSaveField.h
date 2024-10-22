#pragma once

#include <string>

class IntSaveField {
public:
  IntSaveField(std::string name);
  std::string getName() const;
  void setValue(int aValue);
  int getValue() const;

private:
  std::string mName;
  int mValue;
};