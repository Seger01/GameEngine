#pragma once

#include <string>

class IntSaveField {
public:
  IntSaveField(std::string name);
  IntSaveField(std::string name, int value);

  std::string getName() const;

  void setValue(int aValue);

  int getValue() const;

private:
  std::string mName;
  int mValue;
};