#pragma once

#include <string>
#include <type_traits>

template <typename T> class SaveField {
  static_assert(
      std::is_same<T, std::string>::value || std::is_same<T, int>::value ||
          std::is_same<T, float>::value,
      "SaveField can only be instantiated with std::string, int, or float");

public:
  SaveField(std::string name) : mName(name) {}

  std::string getName() const { return mName; }

  void setValue(T aValue) { mValue = aValue; }

  T getValue() const { return mValue; }

private:
  std::string mName;
  T mValue;
};

// Template idea: declare functions of template in allowed types (string, int
// and float), then implement as template in source file.