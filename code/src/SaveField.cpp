#include "SaveField.hpp"

SaveField::SaveField(std::string name) : mName(name) {}

std::string SaveField::getName() const { return mName; }

void SaveField::setValue(std::string aValue) { mValue = aValue; }

std::string SaveField::getValue() const { return mValue; }