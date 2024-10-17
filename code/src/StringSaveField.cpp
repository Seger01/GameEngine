#include "StringSaveField.hpp"

StringSaveField::StringSaveField(std::string name) : mName(name) {}

std::string StringSaveField::getName() const { return mName; }

void StringSaveField::setValue(std::string aValue) { mValue = aValue; }

std::string StringSaveField::getValue() const { return mValue; }