#include "StringSaveField.h"

StringSaveField::StringSaveField(std::string aName) : mName(aName) {}

StringSaveField::StringSaveField(std::string aName, std::string aValue) : mName(aName), mValue(aValue) {}

std::string StringSaveField::getName() const { return mName; }

void StringSaveField::setValue(std::string aValue) { mValue = aValue; }

std::string StringSaveField::getValue() const { return mValue; }