#pragma once

#include <string>

class SaveGameUtil {
public:
  static bool isInteger(const std::string &str);
  static bool isFloat(const std::string &str);
};