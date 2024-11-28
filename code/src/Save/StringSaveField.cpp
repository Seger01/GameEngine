#include "StringSaveField.h"

StringSaveField::StringSaveField(const std::string& aName, const std::string& aValue) : mName(aName), mValue(aValue) {}

std::string StringSaveField::getName() const { return mName; }

void StringSaveField::setValue(const std::string& aValue) { mValue = aValue; }

std::string StringSaveField::getValue() const { return mValue; }