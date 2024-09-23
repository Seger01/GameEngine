#pragma once

#include <string>

class SaveField {
public:
  SaveField(std::string name);

public:
  std::string getName();

public:
  void setValue(std::string aValue);
  std::string getValue();

private:
  std::string mName;
  std::string mValue;
};