#include "StringSaveField.hpp"

StringSaveField::StringSaveField(std::string name) : mName(name) {}

StringSaveField::StringSaveField(std::string name, std::string value)
    : mName(name), mValue(value) {}

std::string StringSaveField::getName() const { return mName; }

void StringSaveField::setValue(std::string aValue) { mValue = aValue; }

std::string StringSaveField::getValue() const { return mValue; }