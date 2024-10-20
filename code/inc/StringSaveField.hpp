#pragma once

#include <string>

class StringSaveField {
public:
  StringSaveField(std::string name);
  StringSaveField(std::string name, std::string value);

  std::string getName() const;

  void setValue(std::string aValue);

  std::string getValue() const;

private:
  std::string mName;
  std::string mValue;
};