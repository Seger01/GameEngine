#pragma once

#include <string>

class SaveField {
public:
  SaveField(std::string name);

public:
  std::string getName() const;

public:
  void setValue(std::string aValue);
  std::string getValue() const;

private:
  std::string mName;
  std::string mValue;
};